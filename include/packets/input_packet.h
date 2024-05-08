#pragma once
#include <cstddef>
#include <vector>
#include "packet.h"

struct InputPacket : public Packet {
    std::vector<int> events;
    float cam_angle;

    size_t calculateSize() const;
    static void serialize(const InputPacket& input, char*& outData);
    static void deserialize(const char* inData, InputPacket& input);
};
