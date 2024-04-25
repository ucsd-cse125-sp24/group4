#pragma once
#include <vector>

// Graphics core
#include "core.h"
#include "enums.h"

struct PlayerState {
    float x, y, z;
    float orientation;
    int score;
    // Add other player-specific state variables
};

struct StudentState {
    float x, y, z;
    float orientation;
    // Add other student-specific state variables
};

struct GameState {
    std::vector<PlayerState> players;
    std::vector<StudentState> students;
    int level;
    // Functions for manipulating the game state
    void updateScores();
    void movePlayer(int playerId, int event, float orientation);
    // Other relevant game state functions
};
