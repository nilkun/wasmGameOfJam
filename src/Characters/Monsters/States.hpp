// #pragma once

// #include "../Monsters.hpp"
// #include "../Player.hpp"

// class Monsters;
// class Monster;
// class Player;

// class State {
//     public:
//         virtual void enter()=0;
//         virtual void execute(const Player &player)=0;
//         virtual void exit()=0;

//         Monster *owner;
//         State() {}
//         State(Monster *own) {
//             owner = own;
//         };
//         ~State() {
//             // delete owner;
//         }
// };

// class StateMachine {
//     public:  
//         // StateMachine() {};      
//         State *state;
//         void setState(State* newState);
//         void changeState(State *newState);
// };


// class Patrol : public State {
//     public:
//         Patrol(Monster *owner) : State(owner) {
//         };

//     void enter();
//     void execute(const Player &player);
//     void exit();
// };

// class ProjectilePatrol : public State {
//     void enter();
//     void execute(const Player &player);
// };

// // THIS IS NOT NECCESARY WITH NEW IMPLEMENTATION
// // class ReturnToPatrol : public State  {
// //     void enter() {  
// //         owner -> setNextTarget(owner -> patrolPath[owner -> patrolIndex].x, owner -> patrolPath[owner -> patrolIndex].y);  
// //     }
// //     void execute(const Player &player) {
// //         if (abs(owner -> position.x - player.position.x) > 3 && abs(owner -> position.y - player.position.y) > 3) {
// //             owner -> setNextTarget(owner -> patrolPath[owner -> patrolIndex].x, owner -> patrolPath[owner -> patrolIndex].y);
// //         }
// //         else {
// //             // switch to chase player state
// //         }
// //     }
// // };

// class ChasePlayer  : public State {
//     void enter();
//     void execute(const Player &player);
// };
