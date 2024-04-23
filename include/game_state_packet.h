#pragma once
#include <vector>
#include "../include/game_state.h"

struct GameStatePacket {
    GameState state; 

    void serialize(const GameStatePacket& packet, char*& outData);
    void deserialize(const char* inData, GameStatePacket& packet);
};
