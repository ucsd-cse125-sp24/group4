#pragma once

struct InputPacket {
    enum InputType { KEYBOARD, MOUSE, KEYBOARD_AND_MOUSE } type;
    union {
        struct {
            int keyCode;  // Represents the key pressed
            bool pressed;  // True if key is pressed, false if released
        } keyboard;
        struct {
            int x, y;  // Mouse coordinates
            int button;  // Button pressed
            bool pressed;  // True if button is pressed, false if released
        } mouse;
    };

    void serialize(const InputPacket& input, char*& outData);
    void deserialize(const char* inData, InputPacket& input);
};
