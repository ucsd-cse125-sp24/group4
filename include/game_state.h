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
    // update player stuff
    void updateScores();
    void movePlayer(int playerId, int event, float orientation);

    // update npc stuff

    // student_update(): for s in students: update

    // student update(current_state): current -> return new_state
    // fill students with the new new_state
    
    // send students to client - servercore

};
