#pragma once

#include "../core.h"
#include "collider.h"

class GameObject {
    protected:
        // glm::mat4 world;
        glm::vec3 old_position;
        glm::vec3 position;     // Position of the object in 3D space
        glm::vec3 velocity; 
        glm::vec3 worldVelocity;   
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
        void setWorldVelocity(glm::vec3 vel) { worldVelocity = vel; }
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
        void applyFriction();
        void applyGravity();
        void applyForce(const glm::vec3& force) {
            this->force += force;
        }
        void makeCollider();
};

class PlayerObject : public GameObject {
    private: 
        short playerId;
        glm::mat4 playerWorld;

    public:

        PlayerObject(AABB* collider) :
            GameObject(collider) {}
        void move();
        void setPlayerId(short id) { playerId = id; }
        short getPlayerId() { return playerId; }
        void setPlayerWorld(glm::mat4 world){ playerWorld = world; }
        glm::mat4 getPlayerWorld() { return playerWorld; }
        void jump();
        void simulate_player(float dt);
        void makeCollider();
};