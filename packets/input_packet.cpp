#include "../include/packets/input_packet.h"
#include <cstring>

size_t InputPacket::calculateSize() const {
    size_t totalSize = 0;
    
    // type, events size, cam angle
    totalSize += sizeof(PacketType) + sizeof(size_t) + sizeof(float);
    // events themselves
    totalSize += sizeof(int) * events.size();

    return totalSize;
}

// Ensure outData is large enough to store all the data that is intended to be serialized.
void InputPacket::serialize(const InputPacket& input, char*& outData) {
    char* temp = outData;

    // serialize packet type
    PacketType type = PLAYER_INPUT;
    memcpy(temp, &type, sizeof(type));
    temp += sizeof(type);
    
    // Serialize size of events
    size_t numEvents = input.events.size();
    memcpy(temp, &numEvents, sizeof(numEvents));
    temp += sizeof(numEvents);

    // Serialize events
    memcpy(temp, input.events.data(), sizeof(int) * numEvents);
    temp += sizeof(int) * numEvents;

    // Serialize cam_angle
    memcpy(temp, &input.cam_angle, sizeof(input.cam_angle));
    temp += sizeof(input.cam_angle);
}

void InputPacket::deserialize(const char* inData, InputPacket& input) {
    // skip packet type; we know it's INPUT if we're calling this
    const char* temp = inData + sizeof(PacketType);

    // Deserialize size of events
    size_t numEvents;
    memcpy(&numEvents, temp, sizeof(numEvents));
    temp += sizeof(numEvents);

    // Deserialize events
    input.events.clear();
    input.events.resize(numEvents);
    memcpy(input.events.data(), temp, sizeof(int) * numEvents);
    temp += sizeof(int) * numEvents;

    // Deserialize cam_angle
    memcpy(&input.cam_angle, temp, sizeof(input.cam_angle));
}
