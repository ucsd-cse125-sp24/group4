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
    const float stepSize = 1.0f;
    const float totalDistance = 5.0f; // Assume this is the total distance for each side of the square

    if (student.distanceMoved < totalDistance)
    {
        glm::vec3 step(0.0f);

        switch (student.currentDir)
        {
        case 0: // Moving up
            step = glm::vec3(0.0f, 0.0f, -stepSize);
            break;
        case 1: // Moving right
            step = glm::vec3(stepSize, 0.0f, 0.0f);
            break;
        case 2: // Moving down
            step = glm::vec3(0.0f, 0.0f, stepSize);
            break;
        case 3: // Moving left
            step = glm::vec3(-stepSize, 0.0f, 0.0f);
            break;
        }

        student.world = glm::translate(student.world, step);
        student.distanceMoved += stepSize;
    }
    else
    {
        student.distanceMoved = 0.0f;
        student.currentDir = (student.currentDir + 1) % 4;

        glm::vec3 currentPosition = glm::vec3(student.world[3][0], student.world[3][1], student.world[3][2]);

        // Create a new rotation matrix
        glm::mat4 newRotation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // Apply rotation to the current matrix without affecting the translation component
        glm::mat4 rotatedMatrix = student.world * newRotation;

        // Set the translation component back to its original
        rotatedMatrix[3] = glm::vec4(currentPosition, 1.0f);

        // Update the student's world matrix
        student.world = rotatedMatrix;


        //  for (int j = 0; j < 4; j++)
        //     {
        //         for (int i = 0; i < 4; i++)
        //         {
        //             std::cout << currentPosition[i][j] << " ";
        //         }
        //         std::cout << std::endl;
        //     }
        // std::cout << "\n\n";

    }
}