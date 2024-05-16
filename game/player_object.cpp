#include "../include/game_object.h"
#define step 1.0f
void PlayerObject::moveForward() {
    glm::vec3 move_force = glm::vec3(0, 0, -step);
    force += move_force;
}

void PlayerObject::moveBackward() {
    glm::vec3 move_force = glm::vec3(0, 0, step);
    force += move_force;
}

void PlayerObject::moveLeft() {
    glm::vec3 move_force = glm::vec3(-step, 0, 0);
    force += move_force;
}

void PlayerObject::moveRight() {
    glm::vec3 move_force = glm::vec3(step, 0, 0);
    force += move_force;
}

void PlayerObject::move(glm::vec3 dir) {
    glm::vec3 move_force = dir * step;
    force += move_force;
}