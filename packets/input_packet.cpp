#include "../include/input_packet.h"
#include <cstring>

// Ensure outData is large enough to store all the data that is intended to be serialized.
void InputPacket::serialize(const InputPacket& input, char*& outData) {
    char* temp = outData;

    // Serialize input type
    memcpy(temp, &input.type, sizeof(input.type));
    temp += sizeof(input.type);

    // Serialize data based on input type
    switch (input.type) {
        case KEYBOARD:
            memcpy(temp, &input.keyboard, sizeof(input.keyboard));
            temp += sizeof(input.keyboard);
            break;
        case MOUSE:
            memcpy(temp, &input.mouse, sizeof(input.mouse));
            temp += sizeof(input.mouse);
            break;
        case KEYBOARD_AND_MOUSE:
            memcpy(temp, &input.keyboard, sizeof(input.keyboard));
            temp += sizeof(input.keyboard);
            memcpy(temp, &input.mouse, sizeof(input.mouse));
            temp += sizeof(input.mouse);
            break;
    }
}

void InputPacket::deserialize(const char* inData, InputPacket& input) {
    // Deserialize input type
    memcpy(&input.type, inData, sizeof(input.type));
    inData += sizeof(input.type);

    // Deserialize data based on input type
    switch (input.type) {
        case KEYBOARD:
            memcpy(&input.keyboard, inData, sizeof(input.keyboard));
            break;
        case MOUSE:
            memcpy(&input.mouse, inData, sizeof(input.mouse));
            break;
        case KEYBOARD_AND_MOUSE:
            memcpy(&input.keyboard, inData, sizeof(input.keyboard));
            inData += sizeof(input.keyboard);
            memcpy(&input.mouse, inData, sizeof(input.mouse));
            break;
    }
}
