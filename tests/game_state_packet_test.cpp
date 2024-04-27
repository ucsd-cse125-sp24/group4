#include <iostream>
#include <vector>
#include <cstring> 
#include <cassert>
#include "../include/game_state_packet.h" 

void setupGameStatePacket(GameStatePacket& packet) {
    // Initialize player states
    packet.state.players.push_back({10.0f, 20.0f, 30.0f, 0.1f, 100}); // Player 1
    packet.state.players.push_back({15.0f, 25.0f, 35.0f, 0.2f, 150}); // Player 2
    packet.state.players.push_back({20.0f, 30.0f, 40.0f, 0.3f, 200}); // Player 3
    packet.state.players.push_back({25.0f, 35.0f, 45.0f, 0.4f, 250}); // Player 4

    // Initialize students states
    packet.state.students.push_back({1.0f, 2.0f, 3.0f, 0.0f}); // Student 1
    packet.state.students.push_back({4.0f, 5.0f, 6.0f, 1.0f}); // Student 2

    packet.state.level = 5;
}

bool compareGameStatePackets(const GameStatePacket& a, const GameStatePacket& b) {
    if (a.state.level != b.state.level) return false;

    for (int i = 0; i < 4; i++) {
        if (a.state.players[i].x != b.state.players[i].x ||
            a.state.players[i].y != b.state.players[i].y ||
            a.state.players[i].z != b.state.players[i].z ||
            a.state.players[i].orientation != b.state.players[i].orientation ||
            a.state.players[i].score != b.state.players[i].score) {
            return false;
        }
    }

    if (a.state.students.size() != b.state.students.size()) return false;

    for (size_t i = 0; i < a.state.students.size(); i++) {
        if (a.state.students[i].x != b.state.students[i].x ||
            a.state.students[i].y != b.state.students[i].y ||
            a.state.students[i].z != b.state.students[i].z ||
            a.state.students[i].orientation != b.state.students[i].orientation) {
            return false;
        }
    }

    return true;
}

void printGameStatePacket(const GameStatePacket& packet) {
    std::cout << "Level: " << packet.state.level << std::endl;
    std::cout << "Players:" << std::endl;
    for (const auto& player : packet.state.players) {
        std::cout << "  x: " << player.x << ", y: " << player.y << ", z: " << player.z
                  << ", orientation: " << player.orientation << ", score: " << player.score << std::endl;
    }
    std::cout << "Students:" << std::endl;
    for (const auto& student : packet.state.students) {
        std::cout << "  x: " << student.x << ", y: " << student.y << ", z: " << student.z
                  << ", orientation: " << student.orientation << std::endl;
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