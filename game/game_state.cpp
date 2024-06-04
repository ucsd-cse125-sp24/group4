#include "../include/game_state.h"

void GameState::updateScores()
{
    // Implementation for updating scores
}

void GameState::moveStudent(StudentState &student, std::vector<PlayerState> players, const float stepSize, const float totalDistance)
{
    // Extract current position from the world matrix
    glm::vec3 currentPos = glm::vec3(student.world[3]);

    // Check for the nearest player within a range of 5 units
    glm::vec3 nearestPlayerPos;
    float minDistance = std::numeric_limits<float>::max();
    bool playerInRange = false;

    for (const auto &p : players)
    {
        glm::vec3 playerPos = glm::vec3(p.world[3]);
        float distance = glm::distance(currentPos, playerPos);

        if (distance < 5.0f && distance < minDistance)
        {
            minDistance = distance;
            nearestPlayerPos = playerPos;
            playerInRange = true;
            student.chasingPlayer = true;
        }
    }

    if (student.chasingPlayer)
    {

        if (student.chaseDuration == 0)
        {
            // Check if player is still in range
            glm::vec3 directionToPlayer = nearestPlayerPos - currentPos;
            if (glm::length(directionToPlayer) < 5.0f)
            {
                student.chaseDuration = 20.0f;
            }
            else
            {
                student.chasingPlayer = false;
                student.chaseDuration = 20.0f;
            }
        }
        else
        {
            // Move towards the player
            glm::vec3 directionToPlayer = nearestPlayerPos - currentPos;
            if (glm::length(directionToPlayer) > 0.0001f)
            { // Check if the direction vector is not zero
                directionToPlayer = glm::normalize(directionToPlayer);
                currentPos += directionToPlayer * stepSize;
                student.chaseDuration -= 1;
            }
        }
    }
    else
    {
        // Move in the current direction
        glm::vec3 step(0.0f);
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
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, -1, 0));
            student.world = rotationMatrix * student.world;
            student.distanceMoved = 0.0f;
            student.currentDir = static_cast<StudentState::Direction>((static_cast<int>(student.currentDir) + 1) % 4);
        }
    }

    student.world[3] = glm::vec4(currentPos, 1.0f);

    if (minDistance < 0.2f) // Assuming this is the threshold for a collision
    {
        student.hasCaughtPlayer = true; 
    }
}