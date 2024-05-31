#include "../include/physics/game_object.h"
#include <cmath>
#define step 30.0f
#define mu 1.4f

void PlayerObject::move() {
    glm::vec3 move_force = glm::vec3(0.0f, 0.0f, -step);
    setForce(force + move_force);
    glm::vec3 norm_force = glm::vec3(0.0f, 0.0f, force.z/abs(force.z) * step);
    setForce(norm_force);
}

void GameObject::applyFriction() {
    glm::vec3 friction = glm::vec3(-velocity.x * mu, 0, -velocity.z * mu);
    setForce(force + friction);
}

void GameObject::applyGravity() {
    glm::vec3 m_gravity = glm::vec3(0, -9.81f, 0);

    if (position.y > 0) {
        setForce(force + m_gravity);
    }   
}

void PlayerObject::jump() {
    velocity.y = 30.0f;
}

void PlayerObject::simulate_player(float dt) {
    
    applyFriction();
    applyGravity();
    glm::vec3 ori_v = velocity;
    velocity += force / mass * dt;
    worldVelocity = glm::vec3(playerWorld * glm::vec4(velocity, 0.0f));
    old_position = position;
    position += worldVelocity * dt;
    // check y >= 0
    // old_position = position;
    // position += 0.5f * (ori_v + velocity) * dt;

    if (position.y < 0) {
        position.y = 0;
        velocity.y = 0;
    }
    
    // printf("forces: <%f, %f, %f>\n", force.x, force.y, force.z);
    // printf("velocity: <%f, %f, %f>\n", velocity.x, velocity.y, velocity.z);
    // printf("positions: <%f, %f, %f>\n\n", position.x, position.y, position.z);

    collider->setBoundingBox(position); // update the bbox based on updated position

    force = glm::vec3(0, 0, 0); // reset net force at the end
}

void PlayerObject::makeCollider() {

    float minX = position.x - PLAYER_WIDTH / 2.0;
    float minY = position.y - PLAYER_LENGTH / 2.0;
    float minZ = position.z;
    float maxX = position.x + PLAYER_WIDTH / 2.0;
    float maxY = position.y + PLAYER_LENGTH / 2.0;
    float maxZ = position.z + PLAYER_HEIGHT;

    collider = new AABB(glm::vec3(minX, minY, minZ), glm::vec3(maxX, maxY, maxZ));
}

void GameObject::makeCollider() {

    float minX = position.x - PLAYER_WIDTH / 2.0;
    float minY = position.y - PLAYER_LENGTH / 2.0;
    float minZ = position.z;
    float maxX = position.x + PLAYER_WIDTH / 2.0;
    float maxY = position.y + PLAYER_LENGTH / 2.0;
    float maxZ = position.z + PLAYER_HEIGHT;

    collider = new AABB(glm::vec3(minX, minY, minZ), glm::vec3(maxX, maxY, maxZ));
}