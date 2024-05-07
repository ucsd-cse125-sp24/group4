#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>
#include <cmath>
#include "../include/game_state.h"

bool checkPosition(const glm::vec3 &expected, const glm::vec3 &actual)
{
    return glm::length(actual - expected) == 0;
}

int main()
{
    GameState state;
    StudentState student;
    student.world = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    student.currentDir = StudentState::Direction::NORTH;
    student.distanceMoved = 0.0f;

    state.students.push_back(student);

    std::vector<glm::vec3> expectedPos = {
        glm::vec3(0, 0, -3), // North 3 steps
        glm::vec3(3, 0, -3), // East 3 steps
        glm::vec3(3, 0, 0),  // South 3 steps
        glm::vec3(0, 0, 0)   // West 3 steps
    };

    // This test checks the case where student starts at the bottom left of the squaren, traverses each side of the square, and returns to the starting point
    for (int x = 0; x < 12; x++)
    {
        state.moveStudent(student, 1, 3);
        for (int j = 0; j < 4; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                std::cout << student.world[i][j] << " ";
            }
            std::cout << std::endl;
        }

        // std::cout << atan2(student.world[0][1],student.world[0][0]) * (180 / 3.141592); // orientation degree?
        std::cout << "\n\n";

        glm::vec3 currentPos = glm::vec3(student.world[3]);

        if (x % 3 == 2)
        { // After moving 3 steps in one direction
            int index = x / 3;
            if (!checkPosition(expectedPos[index], currentPos))
            {
                return 1;
            }
        }
    }

    std::cout << "All tests passed.\n";
    return 0;
}
