#include "../include/input_packet.h"
#include <cstring>

void InputPacket::serialize(const InputPacket& input, char*& outData) {
    // Serialize input type
    memcpy(outData, &input.type, sizeof(input.type));
    outData += sizeof(input.type);

    // Serialize data based on input type
    switch (input.type) {
        case KEYBOARD:
            memcpy(outData, &input.keyboard, sizeof(input.keyboard));
            outData += sizeof(input.keyboard);
            break;
        case MOUSE:
            memcpy(outData, &input.mouse, sizeof(input.mouse));
            outData += sizeof(input.mouse);
            break;
        case KEYBOARD_AND_MOUSE:
            memcpy(outData, &input.keyboard, sizeof(input.keyboard));
            outData += sizeof(input.keyboard);
            memcpy(outData, &input.mouse, sizeof(input.mouse));
            outData += sizeof(input.mouse);
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
