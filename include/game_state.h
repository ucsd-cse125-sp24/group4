#pragma once
#include <vector>

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
    float distanceMoved;
};

struct GameState {
    std::vector<PlayerState> players;
    std::vector<StudentState> students;
    int level;
    int score = 0;
    
    void updateScores();
    void moveStudent(StudentState& student, const float stepSize, const float totalDistance);

};
