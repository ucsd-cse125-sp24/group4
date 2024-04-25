#include "../include/game_state.h"

void GameState::updateScores() {
    // Implementation for updating scores
}

void GameState::movePlayer(int playerId, int event, float orientation) {
    // Implementation for moving a player
    glm::vec3 direction;

    switch (event) {
    case MOVE_FORWARD:
		direction = glm::vec3(0.0f, 0.0f, -1.0f);
		break;
    case MOVE_BACKWARD:
        direction = glm::vec3(0.0f, 0.0f, 1.0f);
        break;
    case MOVE_LEFT:
        direction = glm::vec3(-1.0f, 0.0f, 0.0f);
		break;
    case MOVE_RIGHT:
        direction = glm::vec3(1.0f, 0.0f, 0.0f);
		break;
    }

    // TODO:
    // Use camera orientation as frame of reference

    // Move player in the direction
    
}
