#include <iostream>
#include <cassert>
#include "../include/input_packet.h"

void printInputPacket(const InputPacket &packet)
{
    printf("Events: ");
    for (const auto &event : packet.events)
        printf("%d ", event);
    printf("\n");
    printf("Camera angle: %f\n", packet.cam_angle);
}

bool compareInputPackets(const InputPacket &packet1, const InputPacket &packet2)
{
    return packet1.events == packet2.events && packet1.cam_angle == packet2.cam_angle;
}

int main()
{
    InputPacket input;
    input.events.push_back(0);
    input.events.push_back(1);
    input.events.push_back(2);
    input.cam_angle = 36.0f;
    
    size_t bufferSize = input.calculateSize();
    char *buffer = new char[bufferSize];
    InputPacket::serialize(input, buffer); 

    InputPacket deserializedInput;
    InputPacket::deserialize(buffer, deserializedInput);

    std::cout << "Deserialized Input:" << std::endl;
    printInputPacket(deserializedInput);

    assert(compareInputPackets(input, deserializedInput));

    std::cout << "\nTest passed: Serialized and Deserialized data match." << std::endl;

    return 0;
}
