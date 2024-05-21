#include "../include/physics/game_object.h"
#include <cmath>
#define step 60.0f
#define mu 16

void PlayerObject::move(glm::vec3 dir) {
    glm::vec3 move_force = dir * step;
    setForce(force + move_force);
}

void GameObject::applyFriction() {
    glm::vec3 friction = glm::vec3(-velocity.x * mu, 0, -velocity.z * mu);
    setForce(force + friction);
}

void GameObject::simulate(float dt) {
    applyFriction();
    glm::vec3 ori_v = velocity;
    velocity += force / mass * dt;
    // check y >= 0
    old_position = position;
    position += 0.5f * (ori_v + velocity) * dt;
    
    printf("forces: <%f, %f, %f>\n", force.x, force.y, force.z);
    printf("velocity: <%f, %f, %f>\n", velocity.x, velocity.y, velocity.z);
    printf("positions: <%f, %f, %f>\n\n", position.x, position.y, position.z);
    
    force = glm::vec3(0, 0, 0); // reset net force at the end
    
}