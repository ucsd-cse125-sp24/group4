#include "../include/game_object.h"

void PlayerObject::moveForward() {
    glm::vec3 move_force = glm::vec3(0, 0, -100);
    force += move_force;
}

void PlayerObject::moveBackward() {
    glm::vec3 move_force = glm::vec3(0, 0, 100);
    force += move_force;
}

void PlayerObject::moveLeft() {
    glm::vec3 move_force = glm::vec3(-100, 0, 0);
    force += move_force;
}

void PlayerObject::moveRight() {
    glm::vec3 move_force = glm::vec3(100, 0, 0);
    force += move_force;
}