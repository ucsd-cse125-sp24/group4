#pragma once
#include <vector>

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
    void movePlayer(int playerId, float x, float y, float z);
    // Other relevant game state functions
};
