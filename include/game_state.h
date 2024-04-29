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
    glm::mat4 world;
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
