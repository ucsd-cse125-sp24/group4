#include <iostream>
#include <cassert>
#include "../include/input_packet.h"

void printInputPacket(const InputPacket &packet)
{
    std::cout << "Packet Type: ";
    switch (packet.type)
    {
    case InputPacket::KEYBOARD:
        std::cout << "KEYBOARD" << std::endl;
        std::cout << "Key Code: " << packet.keyboard.keyCode << std::endl;
        std::cout << "Pressed: " << (packet.keyboard.pressed ? "true" : "false") << std::endl;
        break;
    case InputPacket::MOUSE:
        std::cout << "MOUSE" << std::endl;
        std::cout << "X: " << packet.mouse.x << ", Y: " << packet.mouse.y << std::endl;
        std::cout << "Button: " << packet.mouse.button << std::endl;
        std::cout << "Pressed: " << (packet.mouse.pressed ? "true" : "false") << std::endl;
        break;
    case InputPacket::KEYBOARD_AND_MOUSE:
        std::cout << "KEYBOARD_AND_MOUSE" << std::endl;
        std::cout << "Key Code: " << packet.keyboard.keyCode << std::endl;
        std::cout << "Key Pressed: " << (packet.keyboard.pressed ? "true" : "false") << std::endl;
        std::cout << "X: " << packet.mouse.x << ", Y: " << packet.mouse.y << std::endl;
        std::cout << "Button: " << packet.mouse.button << std::endl;
        std::cout << "Button Pressed: " << (packet.mouse.pressed ? "true" : "false") << std::endl;
        break;
    default:
        std::cout << "Unknown" << std::endl;
        break;
    }
}

bool compareInputPackets(const InputPacket &packet1, const InputPacket &packet2)
{
    if (packet1.type != packet2.type)
        return false;
    switch (packet1.type)
    {
    case InputPacket::KEYBOARD:
        return packet1.keyboard.keyCode == packet2.keyboard.keyCode &&
               packet1.keyboard.pressed == packet2.keyboard.pressed;
    case InputPacket::MOUSE:
        return packet1.mouse.x == packet2.mouse.x &&
               packet1.mouse.y == packet2.mouse.y &&
               packet1.mouse.button == packet2.mouse.button &&
               packet1.mouse.pressed == packet2.mouse.pressed;
    case InputPacket::KEYBOARD_AND_MOUSE:
        return packet1.keyboard.keyCode == packet2.keyboard.keyCode &&
               packet1.keyboard.pressed == packet2.keyboard.pressed &&
               packet1.mouse.x == packet2.mouse.x &&
               packet1.mouse.y == packet2.mouse.y &&
               packet1.mouse.button == packet2.mouse.button &&
               packet1.mouse.pressed == packet2.mouse.pressed;
    default:
        return false;
    }
}

int main()
{
    // Test for KEYBOARD
    {
        InputPacket inputKeyboard;
        inputKeyboard.type = InputPacket::KEYBOARD;
        inputKeyboard.keyboard.keyCode = 65; // 'A'
        inputKeyboard.keyboard.pressed = false;

        size_t bufferSize = sizeof(InputPacket);
        char *buffer = new char[bufferSize];
        inputKeyboard.serialize(inputKeyboard, buffer); 

        InputPacket deserializedInputKeyboard;
        deserializedInputKeyboard.deserialize(buffer, deserializedInputKeyboard);

        std::cout << "Deserialized InputPacket (KEYBOARD):" << std::endl;
        printInputPacket(deserializedInputKeyboard);

        assert(compareInputPackets(inputKeyboard, deserializedInputKeyboard));
    }

    // Test for MOUSE
    {
        InputPacket inputMouse;
        inputMouse.type = InputPacket::MOUSE;
        inputMouse.mouse.x = 100;
        inputMouse.mouse.y = 200;
        inputMouse.mouse.button = 1;
        inputMouse.mouse.pressed = true;

        size_t bufferSize = sizeof(InputPacket);
        char *buffer = new char[bufferSize];
        inputMouse.serialize(inputMouse, buffer);

        InputPacket deserializedInputMouse;
        deserializedInputMouse.deserialize(buffer, deserializedInputMouse);

        std::cout << "\nDeserialized InputPacket (MOUSE):" << std::endl;
        printInputPacket(deserializedInputMouse);

        assert(compareInputPackets(inputMouse, deserializedInputMouse));
    }

    // Test for KEYBOARD_AND_MOUSE
    {
        InputPacket inputKeyboardAndMouse;
        inputKeyboardAndMouse.type = InputPacket::KEYBOARD_AND_MOUSE;
        inputKeyboardAndMouse.keyboard.keyCode = 80;
        inputKeyboardAndMouse.keyboard.pressed = false;
        inputKeyboardAndMouse.mouse.x = 150;
        inputKeyboardAndMouse.mouse.y = 300;
        inputKeyboardAndMouse.mouse.button = 1;
        inputKeyboardAndMouse.mouse.pressed = true;

        size_t bufferSize = sizeof(InputPacket);
        char *buffer = new char[bufferSize];
        inputKeyboardAndMouse.serialize(inputKeyboardAndMouse, buffer);

        InputPacket deserializedInputKeyboardAndMouse;
        deserializedInputKeyboardAndMouse.deserialize(buffer, deserializedInputKeyboardAndMouse);

        std::cout << "\nDeserialized InputPacket (KEYBOARD_AND_MOUSE):" << std::endl;
        printInputPacket(deserializedInputKeyboardAndMouse);

        assert(compareInputPackets(inputKeyboardAndMouse, deserializedInputKeyboardAndMouse));
    }

    std::cout << "\nTest passed: Serialized and deserialized data match." << std::endl;

    return 0;
}
