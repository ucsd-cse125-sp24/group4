#pragma once
#include <vector>
#include <chrono>

// Graphics core
#include "core.h"
#include "enums.h"
#include "physics/game_object.h"

struct PlayerState
{
    glm::mat4 world;
    int score;
    // Add other player-specific state variables

    void lose() {
        score = -1;
    }
    int get_score() {
        return score;
    }
};

struct StudentState
{
    enum Direction
    {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    glm::mat4 world;
    Direction currentDir;
    float rotating; // if rotating > 0 means the object is in phase for rotation. rotating = [0,1]
    float distanceMoved;
    float timeSinceLastUpdate;                                 // Time since last update
    std::chrono::high_resolution_clock::time_point lastUpdate; // Time point of last update
    bool chasingPlayer;
    float chaseDuration;
    bool hasCaughtPlayer;
    GameObject* physicalObject;

    StudentState() : timeSinceLastUpdate(0.0f), distanceMoved(0.0f), rotating(0.0f), chasingPlayer(false), chaseDuration(20.0f), hasCaughtPlayer(false)
    {
        world = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        currentDir = NORTH;                                     // Start facing up
        lastUpdate = std::chrono::high_resolution_clock::now(); // Initialize last update time
    }
};

struct GameState
{
    std::vector<PlayerState> players;
    std::vector<StudentState> students;
    int level;
    int score = 0;

    void updateScores();
    void moveStudent(StudentState &student, std::vector<PlayerState> &players, const float stepSize, const float totalDistance);
};
