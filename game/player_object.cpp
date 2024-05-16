#include "../include/game_object.h"
#define step 10
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