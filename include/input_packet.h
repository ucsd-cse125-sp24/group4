#pragma once
#include <vector>

struct InputPacket {
    std::vector<int> events;
    float cam_angle;

    static void serialize(const InputPacket& input, char*& outData);
    static void deserialize(const char* inData, InputPacket& input);
};
