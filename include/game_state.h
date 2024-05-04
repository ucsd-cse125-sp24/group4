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
    int currentDir; // 0 = up, 1 = right, 2 = down, 3 = left
    float distanceMoved;
};

struct GameState {
    std::vector<PlayerState> players;
    std::vector<StudentState> students;
    int level;
    
    void updateScores();
    void movePlayer(int playerId, int event, float orientation);
    void moveStudent(StudentState& student);

};