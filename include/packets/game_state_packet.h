#pragma once
#include <cstddef>
#include <vector>
#include "../game_state.h"
#include "packet.h"

struct GameStatePacket : public Packet {
    GameState state; 

    size_t calculateSize() const;
    static void serialize(const GameStatePacket& packet, char*& outData);
    static void deserialize(const char* inData, GameStatePacket& packet);
};
