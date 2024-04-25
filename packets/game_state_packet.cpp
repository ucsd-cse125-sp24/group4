#include "../include/game_state_packet.h"
#include <cstring>

// Ensure temp is large enough to store all the data that is intended to be serialized.
void GameStatePacket::serialize(const GameStatePacket& packet, char*& outData) {
    char* temp = outData;
    
    // Serialize number of players
    size_t numPlayers = sizeof(packet.state.players) / sizeof(PlayerState);
    memcpy(temp, &numPlayers, sizeof(numPlayers));
    temp += sizeof(numPlayers);

    // Serialize each player's state including x, y, z, orientation, and score
    for (const PlayerState& player : packet.state.players) {
        memcpy(temp, &player.x, sizeof(player.x));
        temp += sizeof(player.x);
        memcpy(temp, &player.y, sizeof(player.y));
        temp += sizeof(player.y);
        memcpy(temp, &player.z, sizeof(player.z));
        temp += sizeof(player.z);
        memcpy(temp, &player.orientation, sizeof(player.orientation));
        temp += sizeof(player.orientation);
        memcpy(temp, &player.score, sizeof(player.score));
        temp += sizeof(player.score);
    }

    // Serialize number of students
    size_t numStudents = packet.state.students.size();
    memcpy(temp, &numStudents, sizeof(numStudents));
    temp += sizeof(numStudents);

    // Serialize each student's state including x, y, z, and orientation
    for (const StudentState& student : packet.state.students) {
        memcpy(temp, &student.x, sizeof(student.x));
        temp += sizeof(student.x);
        memcpy(temp, &student.y, sizeof(student.y));
        temp += sizeof(student.y);
        memcpy(temp, &student.z, sizeof(student.z));
        temp += sizeof(student.z);
        memcpy(temp, &student.orientation, sizeof(student.orientation));
        temp += sizeof(student.orientation);
    }

    // Serialize level
    memcpy(temp, &packet.state.level, sizeof(packet.state.level));
    temp += sizeof(packet.state.level);
}

void GameStatePacket::deserialize(const char* inData, GameStatePacket& packet) {
    // Deserialize number of players
    size_t numPlayers;
    memcpy(&numPlayers, inData, sizeof(numPlayers));
    inData += sizeof(numPlayers);

    for (int i = 0; i < numPlayers; i++) {
        memcpy(&packet.state.players[i].x, inData, sizeof(packet.state.players[i].x));
        inData += sizeof(packet.state.players[i].x);
        memcpy(&packet.state.players[i].y, inData, sizeof(packet.state.players[i].y));
        inData += sizeof(packet.state.players[i].y);
        memcpy(&packet.state.players[i].z, inData, sizeof(packet.state.players[i].z));
        inData += sizeof(packet.state.players[i].z);
        memcpy(&packet.state.players[i].orientation, inData, sizeof(packet.state.players[i].orientation));
        inData += sizeof(packet.state.players[i].orientation);
        memcpy(&packet.state.players[i].score, inData, sizeof(packet.state.players[i].score));
        inData += sizeof(packet.state.players[i].score);
    }

    // Deserialize number of students
    size_t numStudents;
    memcpy(&numStudents, inData, sizeof(numStudents));
    inData += sizeof(numStudents);
    packet.state.students.clear();
    packet.state.students.resize(numStudents);

    // Deserialize each student's state including x, y, z, and orientation
    for (StudentState& student : packet.state.students) {
        memcpy(&student.x, inData, sizeof(student.x));
        inData += sizeof(student.x);
        memcpy(&student.y, inData, sizeof(student.y));
        inData += sizeof(student.y);
        memcpy(&student.z, inData, sizeof(student.z));
        inData += sizeof(student.z);
        memcpy(&student.orientation, inData, sizeof(student.orientation));
        inData += sizeof(student.orientation);
    }

    // Deserialize level
    memcpy(&packet.state.level, inData, sizeof(packet.state.level));
    inData += sizeof(packet.state.level);
}