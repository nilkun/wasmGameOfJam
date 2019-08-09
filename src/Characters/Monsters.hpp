#pragma once

#include <vector>
#include <SDL2/SDL.h>

// #include "./Monsters/States.hpp"
#include "../Functions/Pathfinding.hpp"
#include "../Functions/Nilkun.hpp"
#include "../Graphics/Animation.hpp"
#include "./Player.hpp"

class Player;
class Monster;
class Monsters;

class State {
    public:
        virtual void enter()=0;
        virtual void execute(const Player &player)=0;
        virtual void exit()=0;

        Monster *owner = nullptr;
        State() {}
        State(Monster *own) {
            owner = own;
        };
        ~State() {
            owner = nullptr;
        }
};

class StateMachine {
    public:  
        StateMachine() {};      
        State *state;
        void setState(State* newState, Monster *owner);
        void changeState(State *newState);
};


class Patrol : public State {
    public:
        Patrol(Monster *owner) : State(owner) {
        };

    void enter();
    void execute(const Player &player);
    void exit();
};

class ProjectilePatrol : public State {
    void enter();
    void execute(const Player &player);
};

class ChasePlayer  : public State {
    void enter();
    void execute(const Player &player);
};

class Monster : public StateMachine {
    public:
        Monster(
            const nilkun::Vector position,
            float heading,
            float radius,
            int damage,
            int hitPoints,
            bool isAlive,
            int patrolIndex,
            float speed,
            float projectileSpeed,
            int projectileColor,
            int projectileDamage,
            const Animation walkAnimation,
            const Animation deathAnimation
        ): position(position), 
            heading(heading), 
            radius(radius), 
            damage(damage), 
            m_hitPoints(hitPoints), 
            isAlive(isAlive),
            patrolIndex(patrolIndex),
            m_speed(speed),
            m_projectileSpeed(projectileSpeed),
            m_projectileColor(projectileColor),
            m_projectileDamage(projectileDamage),
            walkAnimation(walkAnimation),
            deathAnimation(deathAnimation)
        {

        }; 
        Monster(const Monster &oldMonster);
        ~Monster() {
            targets.clear();
            patrolPath.clear();
        };
        Monster() {
            // targets.clear();
            // patrolPath.clear();
        };
        bool walksIntoPlayer(Player &player, float x, float y);
        float radius;
        bool isAlive;
        int damage;
        void operator=(const Monster &oldMonster);
        void setMap(std::vector<std::vector<int> > *map);
        // used for moving monster
        void move(nilkun::Vector displacement, Player &player, int currentTime);
        nilkun::Vector getDisplacement(float elapsedTime);
        void hit(int damage);
        void setNewTarget(float x, float y);
        
        Animation walkAnimation;
        Animation deathAnimation;
        nilkun::Vector position;

        std::vector<std::vector<int> > *map;
        nilkun::Vector target;
        std::vector<nilkun::Vector> targets;
        std::vector<nilkun::Vector> patrolPath;

        int patrolIndex;
        float heading;

    private:
        int m_hitPoints;
        float m_speed;
        float m_projectileSpeed;
        int m_projectileColor;
        int m_projectileDamage;
};

struct Projectile {
    float heading;
    nilkun::Vector position;
};

class Monsters {
    private:
    public:  
        std::vector<std::vector<int> > map;
        Monsters() {};
        void init(std::vector<std::vector<int> > &mapp) { map = mapp; };  
        void update(float elapsedTime, int currentTime, Player &player, nilkun::Vector &camera);
        void render(SDL_Renderer* renderer, float tileSize, nilkun::Vector &camera);
        std::vector<Monster> list;
        std::vector<Projectile> projectiles;
};