#pragma once
#include "core.h"
#include "enums.h"
#include <unordered_map>

/*
 * Handles keyboard input
 * Takes in GLFW key tokens: https://www.glfw.org/docs/3.3/group__keys.html
 * and keeps their state (0 or 1, not pressed or pressed) in a map
 * Note: GLFW_RELEASE = 0, GLFW_PRESS = 1, GLFW_REPEAT = 2;
 * 
 * Should output events that are eventually passed to the server.
 * Hardcode mappings for now. Possible support key remapping in the future.
 */

class Input {
private:
	// Maps GLFW_INPUT_KEY to a boolean value
	std::unordered_map<int, bool> keys;

public:
	void update(int key, int action);
	// Returns a VECTOR OF EVENTS (uses an ENUM) to be passed to the server
	std::vector<int> get_action();


};