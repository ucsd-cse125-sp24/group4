#include <iostream>
#include <vector>
#include <cstring> 
#include <cassert>
#include "../include/game_state_packet.h" 

void setupGameStatePacket(GameStatePacket& packet) {
    // Initialize player states
    packet.state.players.push_back({ glm::mat4(1.0f), 100 }); // Player 1
    packet.state.players.push_back({ glm::mat4(2.0f), 150 }); // Player 2
    packet.state.players.push_back({ glm::mat4(3.0f), 200 }); // Player 3
    packet.state.players.push_back({ glm::mat4(4.0f), 250 }); // Player 4

    // Initialize students states
    packet.state.students.push_back({ glm::mat4(1.0f)} ); // Student 1
    packet.state.students.push_back({ glm::mat4(2.0f)} ); // Student 2

    packet.state.level = 5;
}

bool compareGameStatePackets(const GameStatePacket& a, const GameStatePacket& b) {
    if (a.state.level != b.state.level) return false;

    if (a.state.players.size() != b.state.players.size()) return false;

    for (int i = 0; i < 4; i++) {
        if (a.state.players[i].world != b.state.players[i].world ||
            a.state.players[i].score != b.state.players[i].score) {
            return false;
        }
    }

    if (a.state.students.size() != b.state.students.size()) return false;

    for (size_t i = 0; i < a.state.students.size(); i++) {
        if (a.state.students[i].world != b.state.players[i].world) {
            return false;
        }
    }

    return true;
}

void printGameStatePacket(const GameStatePacket& packet) {
    std::cout << "Level: " << packet.state.level << std::endl;
    std::cout << "Players:" << std::endl;
    for (const auto& player : packet.state.players) {
        std::cout << "World: " << std::endl;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
				std::cout << player.world[i][j] << " ";
			}
			std::cout << std::endl;
		}

        std::cout << "Points: " << player.score << std::endl;
    }

    std::cout << "Students:" << std::endl;
    for (const auto& student : packet.state.students) {
        std::cout << "World: " << std::endl;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << student.world[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
}

int main() {
    GameStatePacket originalPacket, deserializedPacket;
    setupGameStatePacket(originalPacket);

    size_t bufferSize = originalPacket.calculateSize();
    char* buffer = new char[bufferSize];

    originalPacket.serialize(originalPacket, buffer);
    deserializedPacket.deserialize(buffer, deserializedPacket);

    // Print original and deserialized packets
    std::cout << "Original Data:" << std::endl;
    printGameStatePacket(originalPacket);

    std::cout << "Deserialized Data:" << std::endl;
    printGameStatePacket(deserializedPacket);

    if (compareGameStatePackets(originalPacket, deserializedPacket)) {
        std::cout << "Test passed: Serialized and deserialized data match." << std::endl;
    } else {
        std::cout << "Test failed: Serialized and deserialized data do not match." << std::endl;
        return 1;
    }

    return 0;
}