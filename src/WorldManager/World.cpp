#include "./World.hpp"

World::World(
    std::vector<SDL_Texture *> tileTextures,
    std::vector<SDL_Texture *> monsterTextures,
    SDL_Texture *playerTexture,
    float tileSize
) : 
    m_playerTexture(playerTexture),
    m_tileSize(tileSize) 
{
    m_player = new Player(playerTexture);
    
    for(int i = 0; i < tileTextures.size(); i++) {
        m_tileTextures.push_back(tileTextures[i]);
    };

    for(int i = 0; i < monsterTextures.size(); i++) {
        m_monsterTextures.push_back(monsterTextures[i]);
    };
    init();
};

void World::init2(
    std::vector<SDL_Texture *> tileTextures,
    std::vector<SDL_Texture *> monsterTextures,
    SDL_Texture *playerTexture,
    float tileSize
) { 
    m_playerTexture = playerTexture,
    m_tileSize = tileSize; 

    m_player = new Player(playerTexture);
    
    for(int i = 0; i < tileTextures.size(); i++) {
        m_tileTextures.push_back(tileTextures[i]);
    };

    for(int i = 0; i < monsterTextures.size(); i++) {
        m_monsterTextures.push_back(monsterTextures[i]);
    };
    init();
}

void World::init() {
    
    // Set some variables;
    int idx = 0;
    int worldWidth = m_importedData[idx++];
    int worldHeight = m_importedData[idx++];
    int screenWidth = m_importedData[idx++];
    int screenHeight = m_importedData[idx];
    int width = worldWidth * screenWidth;
    int height = worldHeight * screenHeight;

    // Set size of map vector
    m_map.resize(width, std::vector<int>(height));

    for(int i = 0; i < worldWidth; i++) {
        for(int j = 0; j < worldHeight; j++) {
            for(int x = 0; x < screenWidth; x++) {
                for(int y = 0; y < screenHeight; y++) {
                    int tileX = x + i * screenWidth;
                    int tileY = y + j * screenHeight;
                    int offsetX = i * screenWidth;
                    int offsetY = j * screenHeight;
                    // Set the tile type
                    m_map[tileX][tileY] = m_importedData[++idx];

                    // Correct tilemap to be traversable 
                    if(m_importedData[idx] == 2) m_map[tileX][tileY] = -2;

                    // Skip the neighbour coordinates
                    ++idx;
                    ++idx;
                    ++idx;
                    ++idx;

                    while(m_importedData[idx + 1] < 0) {
                        int type = m_importedData[++idx];
                        int id = m_importedData[++idx];
                        
                        // Patrol path
                        // std::vector<nilkun::Vector> path;

                        // If type is a monster
                        if(type == -999) {
                            // Add start position to patrol queue

                            // Set some variables for monsters
                            float speed = 1.0f;
                            int hitPoints = 1;
                            int damage = 1;
                            float projectileSpeed = 0.0f;
                            int projectileColor = 0;
                            int projectileDamage = 0;

                            if(id == 1) {
                                speed = 1.6f;
                            }
                            
                            else if(id == 2) {
                                speed = 1.4f;
                                projectileSpeed = 3.0f;
                                projectileColor = 0;
                                // projectileAudio = audio[3]
                                projectileDamage = 1;
                            }
                            // IF THE BOSS
                            else if(id==3) {
                                hitPoints = 4;
                                damage = 2;
                                speed = 2.5f;
                                projectileDamage = 2;
                                projectileSpeed = 5.0f;
                                projectileColor = 1;
                                
                                // monster.projectileAudio = audio[3]
                                // monster.deathAudio = audio[4];
                            }

                            Monster monster(
                                nilkun::Vector{ x + i * screenWidth + .5f, y + j * screenHeight  + .5f }, // position
                                0, // heading
                                8.0f / 32.0f, // radius
                                damage, // damage
                                hitPoints, // hitpoints
                                true, // isAlive
                                0, // patrolIndex
                                speed, //speed
                                projectileSpeed,
                                projectileColor,
                                projectileDamage,
                                Animation (
                                    m_monsterTextures[id * 2], 
                                    nilkun::Vector { 20, 18 }, // sprite size
                                    nilkun::Vector { 10, 8 },  // sprite center
                                    nilkun::Vector { 10 / 20, 10 / 20 }, // offset
                                    playbackModes::LOOP,
                                    8,
                                    5,
                                    0,
                                    true
                                ),
                                Animation (
                                    m_monsterTextures[id * 2 + 1], 
                                    nilkun::Vector { 20, 18 }, // sprite size
                                    nilkun::Vector { 10, 8 },  // sprite center
                                    nilkun::Vector { 10 / 20, 10 / 20 }, // offset
                                    playbackModes::ONCE,
                                    5,
                                    12,
                                    0,
                                    true
                                )                              
                            );
                            int count = 0;
                            while(m_importedData[++idx] != -1) {
                                
                                monster.patrolPath.push_back(nilkun::Vector { static_cast<float>(m_importedData[idx] + offsetX), static_cast<float>(m_importedData[++idx] + offsetY )});
                                
                                count ++;
                            }            
                            if(id==3) {

                                monster.patrolPath = { nilkun::Vector{ 3.0f, 1.0f }, nilkun::Vector{ 3.0f, 8.0f } };
                            }
                            if(monster.patrolPath.size() < 2) monster.patrolPath.push_back(nilkun::Vector { static_cast<float>(tileX), static_cast<float>(tileY) }); 

                            m_monsters.list.push_back(monster); 

                        }
                        else if (type == -998){
                            // m_objects.push_back(nilkun::Vector{x + i * screenWidth + .5f, y + j * screenHeight  + .5f });
                            // ADD JAM JARS!!!
                            ++idx;
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < m_monsters.list.size(); i++) {
        m_monsters.list[i].setMap(&m_map);
        m_monsters.list[i].setState(new Patrol(&m_monsters.list[i]), &m_monsters.list[i]);

    }

}

void World::handleEvents(const Uint8 *keyboard) {
    if(keyboard[SDL_SCANCODE_UP]) {
        if(keyboard[SDL_SCANCODE_LEFT]) { m_player -> changeHeading(PI * 1.25); }
        else if(keyboard[SDL_SCANCODE_RIGHT]) { m_player -> changeHeading(PI * .75); }
        else m_player -> changeHeading(PI);
        m_player -> startMoving(true);
    }
    else if(keyboard[SDL_SCANCODE_DOWN]) {
        if(keyboard[SDL_SCANCODE_LEFT]) { m_player -> changeHeading(PI * 1.75); }
        else if(keyboard[SDL_SCANCODE_RIGHT]) { m_player -> changeHeading(PI * .25); }
        else m_player -> changeHeading(0);
        m_player -> startMoving(true);
    }
    else if(keyboard[SDL_SCANCODE_LEFT]) {
        m_player -> changeHeading(PI * 1.5);
        m_player -> startMoving(true);
    }
    else if(keyboard[SDL_SCANCODE_RIGHT]) {
        m_player -> changeHeading(PI * .5);
        m_player -> startMoving(true);
    }
    else { 
        m_player -> startMoving(false);
    }     
    if(keyboard[SDL_SCANCODE_SPACE]) {
        m_player -> startAttack();
    }
}

void World::render(SDL_Renderer *renderer, int tileSize) {
    for (int x = m_camera.x; x < m_camera.x + 19.0f; x+=1) {
        for(int y = m_camera.y; y < m_camera.y + 11.0f; y+=1) {
            SDL_Rect dest { 
                static_cast<int>((x - m_camera.x) * tileSize), 
                static_cast<int>((y - m_camera.y) * tileSize), 
                tileSize,
                tileSize
            };
            SDL_RenderCopy(renderer, m_tileTextures[abs(m_map[static_cast<int>(x)][static_cast<int>(y)])], NULL, &dest);
        }
    }
    m_monsters.render(renderer, tileSize, m_camera);
    m_player -> render(renderer, tileSize, nilkun::Vector { m_player -> position.x - m_camera.x, m_player -> position.y - m_camera.y });
};

void World::update(float elapsedTime, int currentTime) {
    m_player -> update(elapsedTime, currentTime, m_map, m_monsters);
    moveCamera();
    m_monsters.update(elapsedTime, currentTime, *m_player, m_camera);
}
void World::moveCamera() {
    if(m_player -> position.x <= 10) m_camera.x = 0.0f;
    else if(m_player -> position.x >= 44.0f) m_camera.x = 34.0f;
    else m_camera.x = m_player -> position.x - 10.0f;

    if(m_player -> position.y <= 6.0f) m_camera.y = 0.0f;
    else if(m_player -> position.y >= 25.0f) m_camera.y = 19.0f;
    else m_camera.y = m_player -> position.y - 6.0f;
};
