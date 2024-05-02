#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>
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

    // This test should make student goes up by 5 steps, then turn right and walks for another 5 steps.
    // The first 5 steps seem correct, but the second 5 (rotate to turn right and walk) is not working properly
    for (int x = 0; x < 10; x++)
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
            std::cout << "\n\n";
        }
    }
}