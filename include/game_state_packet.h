#pragma once
#include <vector>
#include "../include/game_state.h"

struct GameStatePacket {
    GameState state; 

    static void serialize(const GameStatePacket& packet, char*& outData);
    static void deserialize(const char* inData, GameStatePacket& packet);
};
