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

void GameState::moveStudent(StudentState &student, std::vector<PlayerState> players, const float stepSize, const float totalDistance)
{

    glm::vec3 step(0.0f);
    glm::vec3 currentPos = glm::vec3(student.world[3]);

    // Check for the nearest player within a range of 2 units
    glm::vec3 nearestPlayerPos;
    float minDistance = std::numeric_limits<float>::max();
    bool playerInRange = false;

    for (const auto& p : players) {
        glm::vec3 playerPos = glm::vec3(p.world[3]);
        float distance = glm::distance(currentPos, playerPos);
        if (distance < 2.0f && distance < minDistance) { // 2.0f should be a constant config value later
            minDistance = distance;
            nearestPlayerPos = playerPos;
            playerInRange = true;
        }
    }

    if (playerInRange) {
        // Player is close, move towards the player
        if (minDistance > 0) {
            glm::vec3 directionToPlayer = glm::normalize(nearestPlayerPos - currentPos);
            step = directionToPlayer * stepSize;
        } else {
            step = glm::vec3(0.0f);
        }
        student.chasingPlayer = true;
        currentPos += step;
        // TODO: calculate rotation matrix
    }
    else if (student.chasingPlayer) {
        // If the player has moved out of range and we were chasing, resume path
        // should implement for it to take the closest, most direct path to the designed path..?
        student.chasingPlayer = false;

    } 
    else {
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

            // Perform rotation
            student.rotating += 0.1f; // change to some constant config later
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(9.0f), glm::vec3(0, -1, 0));
            glm::mat4 orientation = glm::mat4(glm::mat3(student.world));
            student.world = rotationMatrix * orientation; // Apply rotation

            if (student.rotating >= 1){
                student.rotating = 0;
                student.distanceMoved = 0.0f;
                student.currentDir = static_cast<StudentState::Direction>((static_cast<int>(student.currentDir) + 1) % 4); // Change to next direction
            }
            
        }
    }
    // Reapply the correct translation manually
    student.world[3][0] = currentPos.x;
    student.world[3][1] = currentPos.y;
    student.world[3][2] = currentPos.z;
}