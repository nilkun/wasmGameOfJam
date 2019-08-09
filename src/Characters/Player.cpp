#include "./Player.hpp"
#include <iostream>
#include <set>

Player::Player(SDL_Texture *texture) {
    m_animation.init(
        texture, 
        nilkun::Vector { 20, 24 }, // sprite size
        nilkun::Vector { 10, 8 },  // sprite center
        nilkun::Vector { 10 / 20, 10 / 20 }, // offset
        playbackModes::BACKANDFORTH,
        8,
        45,
        7,
        false
    );
    
    position = { 40.5f, 21.5f };
    m_heading = 0;
    radius = 8.0 / 32.0;  
    m_isBouncingBack = false;
    m_speed = 3;
};
// void Player::hit(int damage) {
//     health -= damage;
//     if()
// }
void Player::update(
    float elapsedTime, 
    int currentTime, 
    std::vector<std::vector<int>> &map, 
    Monsters &monsters) 
{
    if(m_isBouncingBack) {
        if(currentTime - m_bounceBackStart >= 300) {
            m_isBouncingBack = false;
        }
        else {
            const float x = position.x + sin(bounceHeading) * 2 * elapsedTime;
            const float y = position.y + cos(bounceHeading) * 2 * elapsedTime;
            if(thereAreNoObstacles(map, x, y) && thereAreNoMonsters(x, y, monsters)) {
                position.x = x;
                position.y = y;
            }
        };
    }
    else if(isMoving) {
        const float x = position.x + sin(m_heading) * m_speed * elapsedTime;
        const float y = position.y + cos(m_heading) * m_speed * elapsedTime;
        if(thereAreNoObstacles(map, x, y)) {
            if(thereAreNoMonsters(x, y, monsters)) {
                position.x = x;
                position.y = y;
            }
            else bounceBack(currentTime, -m_heading);
        }
    };
    if(isAttacking) {
        if(!m_animation.play(elapsedTime)) {
            isAttacking = false;
        }
        else attack(monsters);
    }

}; 
void Player::attack(Monsters &monsters) {
    if(!weaponHasHit) {
        const float width = m_animation.getSize(1.0f / 32.0f).x;
        const float reach = -(8.0f + (7.0f - m_animation.getCurrentFrame())) / 28.0f;

        const float x = position.x + (sin(m_heading) * -reach); + cos(m_heading) * width * .25f;
        const float y = position.y + (cos(m_heading) * -reach); + sin(m_heading) * width * .33f;

        const nilkun::Vector hitArea { x, y };

        for(int i = 0; i < monsters.list.size(); i++) {
            if(monsters.list[i].isAlive 
            && Collision::circleCircle(
                monsters.list[i].position.x,
                monsters.list[i].position.y,
                monsters.list[i].radius,
                hitArea.x,
                hitArea.y,
                0
                )
            ) {
                monsters.list[i].hit(damage);
                weaponHasHit = true;
                break;
            }
        }        
    }
}

void Player::render(SDL_Renderer* renderer, float tileSize, const nilkun::Vector &pos) {
    SDL_Rect src = m_animation.getSource();
    const float scalar = tileSize / 32.0;
    nilkun::Vector center = m_animation.getCenter(scalar);
    nilkun::Vector size = m_animation.getSize(scalar);

    SDL_Rect dest { 
        static_cast<int>(pos.x * tileSize - center.x), 
        static_cast<int>(pos.y * tileSize - center.y), 
        static_cast<int>(size.x),
        static_cast<int>(size.y)
    };
    SDL_Point centerPoint {
        static_cast<int>(center.x),
        static_cast<int>(center.y)
    };

    SDL_RenderCopyEx(renderer, m_animation.getSprites(), &src, &dest, -m_heading * 57.2958, &centerPoint, SDL_FLIP_NONE);
};

void Player::startAttack() {
    if(!isAttacking) {
        isAttacking = true;
        weaponHasHit = false;
        m_animation.rewind();
        m_animation.start();
    }
}

void Player::changeHeading(float radians) {
    m_heading = radians;
}
void Player::startMoving(bool yes) {
    if(yes) isMoving = true;
    else isMoving = false;
}
bool Player::thereAreNoObstacles(std::vector<std::vector<int>> &map, float x, float y) {

    float leftX = x - radius;
    float rightX = x + radius;
    float upY = y - radius;
    float downY = y + radius;

    int left = static_cast<int>(leftX);
    int centerHor = static_cast<int>(x);
    int right = static_cast<int>(rightX);
    int up = static_cast<int>(upY);
    int down = static_cast<int>(downY);
    int centerVert = static_cast<int>(y);
    
    std::set<int> testX, testY;
    testX.insert(static_cast<int>(leftX));
    testX.insert(static_cast<int>(x));
    testX.insert(static_cast<int>(rightX));

    testY.insert(static_cast<int>(upY));
    testY.insert(static_cast<int>(y));
    testY.insert(static_cast<int>(downY));

    for(auto X : testX) {
        for(auto Y : testY) {
            if(map[X][Y] > 0) {
                if(Collision::circleRectangle(x, y, radius, static_cast<float>(X), static_cast<float>(Y), 1.0f, 1.0f)) 
                {
                    return false;
                };
            }

        };
    };
    return true;
};

void Player::isInjured(int damage) {
    health -= damage;
    if(health <= 0) isAlive = false;
}

void Player::bounceBack(int currentTime, float heading) {
    // play audio
    bounceHeading = heading;
    m_isBouncingBack = true;
    m_bounceBackStart = currentTime;

}
bool Player::thereAreNoMonsters(float x, float y, Monsters &monsters) {
    // std::cout << "player: " << radius << " " << monsters.list[0].radius << std::endl;
    for(int i = 0; i < monsters.list.size(); i++) {
        if(Collision::circleCircle(
            x, y, radius, 
            monsters.list[i].position.x, monsters.list[i].position.y, monsters.list[i].radius
            )
            && monsters.list[i].isAlive
        ) {
            isInjured(monsters.list[i].damage);
            return false;
        }
    }
    return true;
}