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
        NORTH = 0,
        EAST = 1,
        SOUTH = 2,
        WEST = 3
    };

    glm::mat4 world;
    Direction currentDir;
    int orientation;
    float distanceMoved;
};

struct GameState {
    std::vector<PlayerState> players;
    std::vector<StudentState> students;
    int level;
    
    void updateScores();
    void movePlayer(int playerId, int event, float orientation);
    void moveStudent(StudentState& student, const float stepSize, const float totalDistance);

};