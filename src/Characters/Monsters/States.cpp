#include "../Monsters.hpp"

void StateMachine::setState(State* newState, Monster* owner) {

    // std::cout << "SETTING STATE" << std::endl;
    state = newState;
    state -> owner = owner;
    // std::cout << "setState()" << std::endl;
    state -> enter();
};
void StateMachine::changeState(State *newState) {
    state -> exit();
    Monster *temp = state -> owner;
    // std::cout << "DELETING STATE" << std::endl;
    // std::cout << state -> owner -> position.x << std::endl;
    delete state;
    // std::cout << "STATE DELETED" << std::endl;
    state = newState;
    state -> owner = temp;
    // std::cout << state -> owner -> position.x << std::endl;
    state -> enter();
};

void Patrol::enter() {
    owner -> setNewTarget(owner -> patrolPath[owner -> patrolIndex].x, owner -> patrolPath[owner -> patrolIndex].y);  
}
void Patrol::execute(const Player &player) {
    if (abs(owner -> position.x - (owner -> patrolPath[owner -> patrolIndex].x + .5f)) < .1f 
     && abs(owner -> position.y - (owner -> patrolPath[owner -> patrolIndex].y + .5f)) < .1f) {
        owner -> patrolIndex++;
        if(owner -> patrolIndex > owner -> patrolPath.size() - 1) 
        {
            owner -> patrolIndex = 0;
        };
        owner -> setNewTarget(owner -> patrolPath[owner -> patrolIndex].x, owner -> patrolPath[owner -> patrolIndex].y);
    }

    // std::cout << "exit!" << std::endl;
    // switch to chase player state
    // else if(abs(owner -> position.x - player.position.x) > 3 && abs(owner -> position.y - player.position.y) > 3) {
    //     owner -> setNewTarget(owner -> patrolPath[owner -> patrolIndex].x, owner -> patrolPath[owner -> patrolIndex].y);
    // }
    // if(abs(owner -> position.x - owner -> target.x) <= .1
    //     && abs(owner -> position.y - owner -> target.y) <= .1
    // ) {
    //     owner -> patrolIndex++;
    //     if(owner -> patrolIndex >= owner -> patrolPath.size()) 
    //     {
    //         owner -> patrolIndex = 0;
    //     }
    //     owner -> setNewTarget(owner -> patrolPath[owner -> patrolIndex].x, owner -> patrolPath[owner -> patrolIndex].y); 
    // }
    // std::cout << "finished comparing" << std::endl;
}
void Patrol::exit() {};

void ProjectilePatrol::enter() {  
    owner -> setNewTarget(owner -> patrolPath[owner -> patrolIndex].x, owner -> patrolPath[owner -> patrolIndex].y);  
}
void ProjectilePatrol::execute(const Player &player) {
    if (abs(owner -> position.x - owner -> patrolPath[owner -> patrolIndex].x - .5) < .1f 
     && abs(owner -> position.y - owner -> patrolPath[owner -> patrolIndex].y - .5) < .1f) {
        owner -> setNewTarget(owner -> patrolPath[owner -> patrolIndex].x, owner -> patrolPath[owner -> patrolIndex].y);
    }
    // switch to chase player state
    else if(abs(owner -> position.x - player.position.x) < 3 && abs(owner -> position.y - player.position.y) < 3) {
        owner -> setNewTarget(owner -> patrolPath[owner -> patrolIndex].x, owner -> patrolPath[owner -> patrolIndex].y);
    }
};


void ChasePlayer::enter() {  
    owner -> setNewTarget(owner -> patrolPath[owner -> patrolIndex].x, owner -> patrolPath[owner -> patrolIndex].y);  
};

void ChasePlayer::execute(const Player &player) {
    if (abs(owner -> position.x - player.position.x) > 3 && abs(owner -> position.y - player.position.y) > 3) {
        owner -> setNewTarget(owner -> patrolPath[owner -> patrolIndex].x, owner -> patrolPath[owner -> patrolIndex].y);
    }
    else {
        // switch to chase player state
    }
};