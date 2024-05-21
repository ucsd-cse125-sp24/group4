#include "../include/physics/game_object.h"
#define step 1.0f
// void PlayerObject::moveForward() {
//     glm::vec3 move_force = glm::vec3(0, 0, -step);
//     force += move_force;
// }

// void PlayerObject::moveBackward() {
//     glm::vec3 move_force = glm::vec3(0, 0, step);
//     force += move_force;
// }

// void PlayerObject::moveLeft() {
//     glm::vec3 move_force = glm::vec3(-step, 0, 0);
//     force += move_force;
// }

// void PlayerObject::moveRight() {
//     glm::vec3 move_force = glm::vec3(step, 0, 0);
//     force += move_force;
// }

void PlayerObject::move(glm::vec3 dir) {
    glm::vec3 move_force = dir * step;
    set_force(force + move_force);
}

void PlayerObject::simulate(float dt) {
    glm::vec3 ori_v = velocity;
    velocity += force / mass * dt;

    // check y >= 0
    old_position = position;
    position += 0.5f * (ori_v + velocity) * dt;
    printf("positions: <%f, %f, %f>\n", position.x, position.y, position.z);
    force = glm::vec3(0, 0, 0); // reset net force at the end
    //obj->velocity = glm::vec3(0, 0, 0);
    printf("forces: <%f, %f, %f>\n", force.x, force.y, force.z);
}