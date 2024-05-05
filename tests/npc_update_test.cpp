#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>
#include <cmath>
#include "../include/game_state.h"

// To run the test:
// g++ -Iinclude -o testNPC tests/npc_update_test.cpp game/game_state.cpp
// ./testNPC

int main()
{

    GameState state;

    StudentState student;
    student.world = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    student.currentDir = 0; // Start facing up
    student.distanceMoved = 0.0f;

    state.students.push_back(student);

    // This test checks the case where student starts at the bottom left of the squaren, traverses each side of the square, and returns to the starting point
    for (int x = 0; x < 20; x++)
    {
        for (StudentState& s : state.students)
        {
            state.moveStudent(s);
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    std::cout << s.world[i][j] << " ";
                }
                std::cout << std::endl;
            }
            // degree is:
            std::cout << atan2(s.world[0][1],s.world[0][0]) * (180 / 3.141592);
            std::cout << "\n\n";
        }
    }
}
