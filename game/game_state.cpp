#include "../include/game_state.h"

void GameState::updateScores()
{
    // Implementation for updating scores
    score += 1;
}

void GameState::moveStudent(StudentState &student, const float stepSize, const float totalDistance)
{

    glm::vec3 step(0.0f);
    glm::vec3 currentPos = glm::vec3(student.world[3]);

    switch (student.currentDir)
    {
    case StudentState::Direction::NORTH: // decrement z
        step = glm::vec3(0.0f, 0.0f, -stepSize);
        break;
    case StudentState::Direction::EAST: // increment x
        step = glm::vec3(stepSize, 0.0f, 0.0f);
        break;
    case StudentState::Direction::SOUTH: // increment z
        step = glm::vec3(0.0f, 0.0f, stepSize);
        break;
    case StudentState::Direction::WEST: // decrement x
        step = glm::vec3(-stepSize, 0.0f, 0.0f);
        break;
    }

    currentPos += step;
    student.distanceMoved += stepSize;

    if (student.distanceMoved >= totalDistance)
    {
        student.distanceMoved = 0.0f;
        student.currentDir = static_cast<StudentState::Direction>((static_cast<int>(student.currentDir) + 1) % 4); // Change to next direction

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