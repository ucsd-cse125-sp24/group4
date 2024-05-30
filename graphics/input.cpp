#include "../include/input.h"
#include "input.h"

void Input::update(int key, int action) {
	keys[key] = action;
}

bool Input::isKeyPressed(int key) const {
    auto it = keys.find(key);
    if (it != keys.end() && it->second == GLFW_PRESS) {
        return true;
    }
    return false;
}

std::vector<int> Input::get_action() {
	std::vector<int> events;
	if (keys[GLFW_KEY_W]) {
		if (!keys[GLFW_KEY_S]) events.push_back(MOVE_FORWARD);
	}
	else if (keys[GLFW_KEY_S]) {
		events.push_back(MOVE_BACKWARD);
	}

	if (keys[GLFW_KEY_A]) {
		if (!keys[GLFW_KEY_D]) events.push_back(MOVE_LEFT);
	}
	else if (keys[GLFW_KEY_D]) {
		events.push_back(MOVE_RIGHT);
	}
	else if (keys[GLFW_KEY_SPACE]) {
		events.push_back(JUMP);
	}
	else if (keys[GLFW_KEY_LEFT_CONTROL]) {
		events.push_back(DROP);
	}

	return events;
}	