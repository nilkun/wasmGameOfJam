#include "./Monsters.hpp"
#include <iostream>

Monster::Monster(const Monster &oldMonster) {

        radius = oldMonster.radius;
        isAlive = oldMonster.isAlive;
        damage = oldMonster.damage;
        map = oldMonster.map;
       

        walkAnimation = Animation(oldMonster.walkAnimation);
        deathAnimation = Animation(oldMonster.deathAnimation);
        position = nilkun::Vector{ oldMonster.position.x, oldMonster.position.y };


        target = nilkun::Vector{oldMonster.target.x, oldMonster.target.y };
        for(int i = 0; i < oldMonster.targets.size(); i++) {
            targets.push_back(nilkun::Vector{oldMonster.targets[i].x, oldMonster.targets[i].y });
        };
        for(int i = 0; i < oldMonster.patrolPath.size(); i++) {
            patrolPath.push_back(nilkun::Vector{ oldMonster.patrolPath[i].x, oldMonster.patrolPath[i].y });
        };
        
        patrolIndex = oldMonster.patrolIndex;
        heading = oldMonster.heading;
        m_hitPoints = oldMonster.m_hitPoints;

        m_speed = oldMonster.m_speed;
        m_projectileSpeed = oldMonster.m_projectileSpeed;
        m_projectileColor = oldMonster.m_projectileColor;

        m_projectileDamage = oldMonster.m_projectileDamage;




}

void Monster::setMap(std::vector<std::vector<int>> *mapp) {
    map = mapp;
}

void Monster::hit(int damage) {
    m_hitPoints -= damage;
    if(m_hitPoints <= 0) {  
        deathAnimation.rewind();
        isAlive = false;
    }
}
nilkun::Vector Monster::getDisplacement(float elapsedTime) {
    nilkun::Vector direction { target.x + .5f - position.x, target.y + .5f - position.y };

    const double PI = 3.141592653589793238462643383279502884L;

    if(abs(direction.x) < .05f && abs(direction.y) < .05f) {
        targets.pop_back();
        if(targets.size() == 0) {
        }
        else {
            target = { targets[targets.size() - 1].x, targets[targets.size() - 1].y }; 
            direction.x = target.x + .5f - position.x; 
            direction.y = target.y + .5f - position.y;
        }

    }
    
    if(direction.x >= .05f) {
        if(direction.y >= .05f) { heading = PI * 0.25; }
        else if(direction.y <= -.05f) { heading = PI * .75; }
        else heading = PI * .5f;
    }
    else if(direction.x <= -.05f) {
        if(direction.y >= .05f) { heading = PI * 1.75; }
        else if(direction.y <= -.05f) { heading = PI * 1.25; }
        else heading = PI * 1.5f;
    }
    else if(direction.y >= .05f) {
        heading = 0;
    }
    else if(direction.y <= -.05f) {
        heading = PI;
    }
    
    return nilkun::Vector { sin(heading) * elapsedTime, cos(heading) * elapsedTime };
};
void Monster::setNewTarget(float x, float y) {
    targets.clear();
    targets = Pathfinding::astar(*map, position, nilkun::Vector { x, y });
    targets.pop_back();
    target = targets[targets.size() - 1];
};
void Monster::operator=(const Monster &oldMonster ) { 
        radius = oldMonster.radius;
        position = nilkun::Vector{ oldMonster.position.x, oldMonster.position.y };
        isAlive = oldMonster.isAlive;
        damage = oldMonster.damage;

        walkAnimation = Animation(oldMonster.walkAnimation);
        deathAnimation = Animation(oldMonster.deathAnimation);
        map = oldMonster.map;

        target = nilkun::Vector{oldMonster.target.x, oldMonster.target.y };

        targets.clear();
        patrolPath.clear();
        for(int i = 0; i < oldMonster.targets.size(); i++) {
            targets.push_back(nilkun::Vector{ oldMonster.targets[i].x, oldMonster.targets[i].y } );
        };

        for(int i = 0; i < oldMonster.patrolPath.size(); i++) {
            patrolPath.push_back(nilkun::Vector{ oldMonster.patrolPath[i].x, oldMonster.patrolPath[i].y });
        };
        
        patrolIndex = oldMonster.patrolIndex;
        heading = oldMonster.heading;
        m_hitPoints = oldMonster.m_hitPoints;
        m_speed = oldMonster.m_speed;
        m_projectileSpeed = oldMonster.m_projectileSpeed;
        m_projectileColor = oldMonster.m_projectileColor;
        m_projectileDamage = oldMonster.m_projectileDamage;
}

bool Monster::walksIntoPlayer(Player &player, float x, float y) {
    return Collision::circleCircle(x, y, radius, player.position.x, player.position.y, player.radius);
};

void Monster::move(nilkun::Vector displacement, Player &player, int currentTime) {
    const float x = position.x + displacement.x;
    const float y = position.y += displacement.y;
    if(!walksIntoPlayer(player, x, y)) {
        position.x = x; position.y = y;
    } 
    else {
        player.isInjured(damage);
        player.bounceBack(currentTime, heading);
    }
}

void Monsters::update(float elapsedTime, int currentTime, Player &player, nilkun::Vector &camera) {
    auto i = list.begin();
    while(i != list.end()) {
        // int a = 0;
        if(
            abs(i -> position.x - (camera.x + 10.0f)) < 10.0f 
            && abs(i -> position.y - (camera.y + 6.0f)) < 6.0f
        ) {
            if(i -> isAlive) {
                i -> move(i -> getDisplacement(elapsedTime), player, currentTime); 
                i -> state -> execute(player);
                i -> walkAnimation.play(elapsedTime); 
                ++i; 
                // ++a;         
            }
            else if(i -> deathAnimation.play(elapsedTime)) {
                    ++i;                
                    // ++a;                
            }
            else {
                i = list.erase(i);
            }
        }
        else {
            ++i;

            // ++a;  
        };
    }
}

void Monsters::render(
    SDL_Renderer* renderer, 
    float tileSize, 
    nilkun::Vector &camera
) {
    for(int i = 0; i < list.size(); i++) {
        if(
            abs(list[i].position.x - (camera.x + 10.0f)) < 10.0f 
            && abs(list[i].position.y - (camera.y + 6.0f)) < 6.0f
        ) {
            Animation animation;
            if(list[i].isAlive) animation = list[i].walkAnimation;
            else animation = list[i].deathAnimation;

            SDL_Rect src = animation.getSource();
            const float scalar = tileSize / 32.0;
            nilkun::Vector center = animation.getCenter(scalar);
            nilkun::Vector size = animation.getSize(scalar);

            SDL_Rect dest { 
                static_cast<int>((list[i].position.x - camera.x) * tileSize - center.x), 
                static_cast<int>((list[i].position.y - camera.y) * tileSize - center.y), 
                static_cast<int>(size.x),
                static_cast<int>(size.y)
            };

            SDL_Point centerPoint {
                static_cast<int>(center.x),
                static_cast<int>(center.y)
            };
            SDL_RenderCopyEx(renderer, animation.getSprites(), &src, &dest, -list[i].heading * 57.2958, &centerPoint, SDL_FLIP_NONE);
        }
    }
};
