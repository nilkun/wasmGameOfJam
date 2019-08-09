#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <iostream>
#include "./Monsters.hpp"
#include "../Functions/Nilkun.hpp"
#include "../Functions/Collision.hpp"
#include "../Graphics/Animation.hpp"

class Monsters;
class Monster;

class Player {
    public: 

        Player(SDL_Texture* texture);
        void update(
            float elapsedTime, 
            int currentTime,
            std::vector<std::vector<int> > &map, 
            Monsters &monsters
        );          
        // void hit(int damage);
        void changeHeading(float radians);
        void startMoving(bool yes);
        void startAttack();
        
        float radius;
        nilkun::Vector position;
        void render(SDL_Renderer* renderer, float tileSize, const nilkun::Vector &pos);
        float bounceHeading;


        void isInjured(int damage);
        void bounceBack(int currentTime, float heading);
        
    private:
        Animation m_animation;  
        float m_heading;
        int m_bounceBackStart;
        bool m_isBouncingBack;
        float m_speed;

        bool isMoving = false;
        bool isAttacking = false;
        bool weaponHasHit = false;
        bool isAlive = true;
        float health = 5;
        int damage = 1;
        bool thereAreNoObstacles(std::vector<std::vector<int> > &map, float x, float y);

    private:
        // float maxHealth = 5;
        // int collectedItems = 0; // Need to collect all 7
        // float swordDamage = 1;
        // nilkun::Vector heading { 0, 0 };
        // animation struct
        // death animation
        // float reach = 0;
        // bool isAlive = true;
        // bool isStabbing = false;
        bool thereAreNoMonsters(float x, float y, Monsters &monsters);
        void attack(Monsters &monsters); // CHECKS IF YOU HIT MONSTER WITH SWORD
        // std::vector<Projectile> projectiles;


        void init();

};
