#pragma once

#include "core.h"
#include "colliders.h"

struct GameObject {

    // have a variable for type player 
    glm::mat4 world;
    glm::vec3 old_position;
    glm::vec3 position;    // Position of the object in 3D space
    glm::vec3 velocity;    // Velocity of the object
    glm::vec3 force; // Acceleration of the object
    float mass;            // Mass of the object
    //Collider* collider; // bounding volume/box
    glm::vec3 minBound; // min coords of the bounding box
    glm::vec3 maxBound; // max coords of the bounding box

    // Constructor to initialize properties
    //GameObject(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float m, Collider* c);
};

struct PlayerObject : GameObject {
    short playerId;

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void move(glm::vec3 dir);
};