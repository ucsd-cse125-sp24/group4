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
    enum Direction {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    glm::mat4 world;
    Direction currentDir;
    float rotating; // if rotating > 0 means the object is in phase for rotation. rotating = [0,1]
    float distanceMoved;
    float timeSinceLastUpdate; // Time since last update
    std::chrono::high_resolution_clock::time_point lastUpdate; // Time point of last update
    bool chasingPlayer;

    StudentState() : timeSinceLastUpdate(0.0f) {
        world = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        currentDir = NORTH; // Start facing up
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
    void moveStudent(StudentState& student, std::vector<PlayerState> players, const float stepSize, const float totalDistance);

};
