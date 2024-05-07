#pragma once
#include <vector>
#include <chrono>

// Graphics core
#include "core.h"
#include "enums.h"

struct PlayerState {
    glm::mat4 world;
    int score;
    // Add other player-specific state variables
};

struct StudentState {
    glm::mat4 world;
    int currentDir; // 0 = up, 1 = right, 2 = down, 3 = left
    float distanceMoved;
    float timeSinceLastUpdate; // Time since last update
    std::chrono::high_resolution_clock::time_point lastUpdate; // Time point of last update

    StudentState() : timeSinceLastUpdate(0.0f) {
        world = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        currentDir = 0; // Start facing up
        distanceMoved = 0.0f;
        timeSinceLastUpdate = 0.0f;
        lastUpdate = std::chrono::high_resolution_clock::now(); // Initialize last update time
    }
};

struct GameState {
    std::vector<PlayerState> players;
    std::vector<StudentState> students;
    int level;
    
    void updateScores();
    void movePlayer(int playerId, int event, float orientation);
    void moveStudent(StudentState& student);

};