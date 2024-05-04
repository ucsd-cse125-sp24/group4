#include "../include/game_state.h"

void GameState::updateScores()
{
    // Implementation for updating scores
}

void GameState::movePlayer(int playerId, int event, float orientation)
{
    // Implementation for moving a player
    glm::vec3 direction;

    switch (event)
    {
    case MOVE_FORWARD:
        direction = glm::vec3(0.0f, 0.0f, -1.0f);
        break;
    case MOVE_BACKWARD:
        direction = glm::vec3(0.0f, 0.0f, 1.0f);
        break;
    case MOVE_LEFT:
        direction = glm::vec3(-1.0f, 0.0f, 0.0f);
        break;
    case MOVE_RIGHT:
        direction = glm::vec3(1.0f, 0.0f, 0.0f);
        break;
    }

    // TODO:
    // Use camera orientation as frame of reference

    // Move player in the direction
}

void GameState::moveStudent(StudentState &student)
{
    const float stepSize = 1.0f; // Assume 1 step at a time
    const float totalDistance = 5.0f; // Assume each side of the square is 5

    glm::vec3 step(0.0f);
    glm::vec3 currentPos = glm::vec3(student.world[3]);

    switch (student.currentDir)
    {
    case 0: // Moving up (decrement z)
        step = glm::vec3(0.0f, 0.0f, -stepSize);
        break;
    case 1: // Moving right (increment x)
        step = glm::vec3(stepSize, 0.0f, 0.0f);
        break;
    case 2: // Moving down (increment z)
        step = glm::vec3(0.0f, 0.0f, stepSize);
        break;
    case 3: // Moving left (decrement x)
        step = glm::vec3(-stepSize, 0.0f, 0.0f);
        break;
    }

    currentPos += step;
    student.distanceMoved += stepSize;

    if (student.distanceMoved >= totalDistance)
    {
        student.distanceMoved = 0.0f;
        student.currentDir = (student.currentDir + 1) % 4; // Change to next direction

        // Perform rotation
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 0, 1));
        glm::mat4 orientation = glm::mat4(glm::mat3(student.world));
        student.world = rotationMatrix * orientation; // Apply rotation
    }
    // Reapply the correct translation manually
    student.world[3][0] = currentPos.x;
    student.world[3][1] = currentPos.y;
    student.world[3][2] = currentPos.z;
}