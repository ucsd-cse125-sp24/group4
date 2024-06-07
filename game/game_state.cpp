#include "../include/game_state.h"

void GameState::updateScores()
{
    // Implementation for updating scores
    score += 1;
}

void GameState::setScores(int new_score)
{
    score = new_score;
}

void GameState::moveStudent(StudentState &student, std::vector<PlayerState> &players, const float stepSize, const float totalDistance)
{
    // Extract current position from the world matrix
    glm::vec3 currentPos = glm::vec3(student.world[3]);

    // Check for the nearest player within a range of 5 units
    glm::vec3 nearestPlayerPos;
    float minDistance = std::numeric_limits<float>::max();
    bool playerInRange = false;

    for (auto &p : players)
    {
        glm::vec3 playerPos = glm::vec3(p.world[3]);
        float distance = glm::distance(currentPos, playerPos);

        if (p.score == -1) {
            distance = std::numeric_limits<float>::max();
        }
        
        float x_diff = abs(currentPos.x - playerPos.x);
        float z_diff = abs(currentPos.z - playerPos.z);
        
        if (x_diff < 1.0f && z_diff < 1.0f && playerPos.y < 6) // Assuming this is the threshold for a collision
        {
            student.chasingPlayer = false;
            p.lose();
            return;
        }

        if (distance <= 20.0f && distance < minDistance)
        {
            // printf("positions: <%f, %f, %f>\n\n", x_diff, z_diff, playerPos.y);
            // printf("positions: <%f, %f, %f>\n", playerPos.x, playerPos.y, playerPos.z);
            // printf("positions: <%f, %f, %f>\n\n", currentPos.x, currentPos.y, currentPos.z);
            minDistance = distance;
            nearestPlayerPos = playerPos;
            playerInRange = true;
            student.chasingPlayer = true;
        }
    }

    if (student.chasingPlayer)
    {
        //std::cout << "The number is: " << student.chaseDuration << std::endl;
        glm::vec3 directionToPlayer = nearestPlayerPos - currentPos;
        if (student.chaseDuration == 0)
        {
            // Check if player is still in range
            if (glm::length(directionToPlayer) > 20.0f)
            {
                student.chasingPlayer = false;
            }
            student.chaseDuration = 20.0f;
        }
        else
        {
            // Move towards the player
            directionToPlayer = glm::normalize(directionToPlayer);
            directionToPlayer.y = 0;
            currentPos += directionToPlayer * stepSize;
            student.chaseDuration -= 1;

            // Update student's facing direction
            glm::vec3 forward = glm::normalize(-directionToPlayer); // Change here to face the player correctly
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 right = glm::cross(up, forward);

            // Create a new rotation matrix
            glm::mat4 rotationMatrix(1.0f);
            rotationMatrix[0] = glm::vec4(right, 0.0f);
            rotationMatrix[1] = glm::vec4(up, 0.0f);
            rotationMatrix[2] = glm::vec4(forward, 0.0f);

            // Preserve the current scale
            glm::vec3 currentScale = glm::vec3(glm::length(student.world[0]),
                                               glm::length(student.world[1]),
                                               glm::length(student.world[2]));

            // Apply the scale to the rotation matrix
            rotationMatrix = glm::scale(rotationMatrix, currentScale);

            // // Set the student's world matrix with the new rotation and the current position
            student.world[0] = rotationMatrix[0];
            student.world[1] = rotationMatrix[1];
            student.world[2] = rotationMatrix[2];
            // student.world[3] = glm::vec4(currentPos, 1.0f);
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
}