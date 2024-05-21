#pragma once

#include "../core.h"
#include "collider.h"

class GameObject {
    protected:
        // glm::mat4 world;
        glm::vec3 old_position;
        glm::vec3 position;     // Position of the object in 3D space
        glm::vec3 velocity;    
        glm::vec3 force;        // Acceleration of the object
        float mass;             // Mass of the object
        Collider* collider;

    public:

        GameObject(Collider* collider) :
            position(glm::vec3(0.0f)),
            old_position(glm::vec3(0.0f)),
            velocity(glm::vec3(0.0f)),
            force(glm::vec3(0.0f)),
            mass(10.0),
            collider(collider) {}
        // update position of the object based on forces for delta time tick
        void simulate(float dt);

        // setters and getters
        void setPosition(glm::vec3 pos) { position = pos; }
        void setOldPosition(glm::vec3 pos) { old_position = pos; }
        void setForce(glm::vec3 f) { force = f; }
        void setVelocity(glm::vec3 vel) { velocity = vel; }
        void setMass(float m) { mass = m; }
        void setCollider(Collider* col) { collider = col; }
        

        glm::vec3 getPosition() { return position; }
        glm::vec3 getOldPosition() { return old_position; }
        glm::vec3 getForce() { return force; }
        glm::vec3 getVelocity() { return velocity; }
        float getMass() { return mass; }
        Collider& getCollider() {
            // TBD    transform?
            return *collider;
        }

};

class PlayerObject : public GameObject {
    private: 
        short playerId;

    public:

        PlayerObject(Collider* collider) :
            GameObject(collider) {}
        void move(glm::vec3 dir);
        void setPlayerId(short id) { playerId = id; }
        short getPlayerId() { return playerId; }
};


// struct GameObject {

//     // have a variable for type player 
//     glm::mat4 world;
//     glm::vec3 old_position;
//     glm::vec3 position;    // Position of the object in 3D space
//     glm::vec3 velocity;    // Velocity of the object
//     glm::vec3 force; // Acceleration of the object
//     float mass;            // Mass of the object
//     //Collider* collider; // bounding volume/box

//     // Constructor to initialize properties
//     //GameObject(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float m, Collider* c);
// };

// struct PlayerObject : GameObject {
//     short playerId;

//     void moveForward();
//     void moveBackward();
//     void moveLeft();
//     void moveRight();
//     void move(glm::vec3 dir);
// };