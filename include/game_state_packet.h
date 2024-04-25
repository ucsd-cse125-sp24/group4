#pragma once
#include <cstddef>
#include <vector>
#include "../include/game_state.h"

struct GameStatePacket {
    GameState state; 

    size_t calculateSize() const;
    static void serialize(const GameStatePacket& packet, char*& outData);
    static void deserialize(const char* inData, GameStatePacket& packet);
};
