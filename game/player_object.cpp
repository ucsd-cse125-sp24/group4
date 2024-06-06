#include "../include/physics/game_object.h"
#include <cmath>
#define step 45.0f
#define mu 1.2f

void PlayerObject::move() {
    glm::vec3 move_force = glm::vec3(0.0f, 0.0f, -step);
    setForce(force + move_force);
    glm::vec3 norm_force = glm::vec3(0.0f, 0.0f, force.z/abs(force.z) * step);
    setForce(norm_force);
}

void GameObject::moveNPC(const glm::vec3 directionToPlayer, const float stepSize) {
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

    velocity += force / mass * dt;
    worldVelocity = glm::vec3(playerWorld * glm::vec4(velocity, 0.0f));
    old_position = position;
    position += worldVelocity * dt;

    if (position.y < 0) {
        position.y = 0;
        velocity.y = 0;
    }
    
    // printf("forces: <%f, %f, %f>\n", force.x, force.y, force.z);
    // printf("velocity: <%f, %f, %f>\n", velocity.x, velocity.y, velocity.z);
    // printf("positions: <%f, %f, %f>\n\n", position.x, position.y, position.z);

    collider->setBoundingBox(position, TYPE_PLAYER); // update the bbox based on updated position

    force = glm::vec3(0, 0, 0); // reset net force at the end
}

void GameObject::simulate(float dt, glm::mat4 NPC_world) {
    
    // applyFriction();
    // applyGravity();

    worldVelocity = glm::vec3(NPC_world * glm::vec4(velocity, 0.0f));
    old_position = position;
    position += worldVelocity * dt;

    collider->setBoundingBox(position, TYPE_NPC); // update the bbox based on updated position
    printf("old positions: <%f, %f, %f>\n", old_position.x, old_position.y, old_position.z);
    printf("positions: <%f, %f, %f>\n", position.x, position.y, position.z);
    printf("world velocity: <%f, %f, %f>\n\n", worldVelocity.x, worldVelocity.y, worldVelocity.z);
    printf("velocity: <%f, %f, %f>\n\n", velocity.x, velocity.y, velocity.z);
    force = glm::vec3(0, 0, 0); // reset net force at the end

}
