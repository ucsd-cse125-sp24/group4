#include <iostream>
#include <vector>
#include <cstring> 
#include <cassert>
#include "../include/game_state_packet.h" 

void setupGameStatePacket(GameStatePacket& packet) {
    // Initialize player states
    for (int i = 0; i < 4; i++) {
        packet.state.players[i].x = i * 1.0f;
        packet.state.players[i].y = i * 2.0f;
        packet.state.players[i].z = i * 3.0f;
        packet.state.players[i].orientation = i * 10.0f;
        packet.state.players[i].score = i * 100;
    }

    // Initialize students states
    packet.state.students.push_back({1.0f, 2.0f, 3.0f, 0.0f});
    packet.state.students.push_back({4.0f, 5.0f, 6.0f, 1.0f});
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

    // Calculate the size needed for the serialization buffer
    size_t bufferSize = sizeof(size_t) + // Number of players
                        4 * (sizeof(float) * 3 + sizeof(float) + sizeof(int)) + // Player data
                        sizeof(size_t) + // Number of students
                        originalPacket.state.students.size() * (sizeof(float) * 3 + sizeof(float)) + // Student data
                        sizeof(int); // Level

    char* buffer = new char[bufferSize];
    char* bufferStart = buffer;

    originalPacket.serialize(originalPacket, buffer);

    buffer = bufferStart; 
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

    delete[] bufferStart;

    return 0;
}