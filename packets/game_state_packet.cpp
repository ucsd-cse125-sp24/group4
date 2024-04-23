#include "../include/game_state_packet.h"
#include <cstring>

void GameStatePacket::serialize(const GameStatePacket& packet, char*& outData) {
    // Serialize number of players
    size_t numPlayers = sizeof(packet.state.players) / sizeof(PlayerState);
    memcpy(outData, &numPlayers, sizeof(numPlayers));
    outData += sizeof(numPlayers);

    // Serialize each player's state including x, y, z, orientation, and score
    for (const PlayerState& player : packet.state.players) {
        memcpy(outData, &player.x, sizeof(player.x));
        outData += sizeof(player.x);
        memcpy(outData, &player.y, sizeof(player.y));
        outData += sizeof(player.y);
        memcpy(outData, &player.z, sizeof(player.z));
        outData += sizeof(player.z);
        memcpy(outData, &player.orientation, sizeof(player.orientation));
        outData += sizeof(player.orientation);
        memcpy(outData, &player.score, sizeof(player.score));
        outData += sizeof(player.score);
    }

    // Serialize number of students
    size_t numStudents = packet.state.students.size();
    memcpy(outData, &numStudents, sizeof(numStudents));
    outData += sizeof(numStudents);

    // Serialize each student's state including x, y, z, and orientation
    for (const StudentState& student : packet.state.students) {
        memcpy(outData, &student.x, sizeof(student.x));
        outData += sizeof(student.x);
        memcpy(outData, &student.y, sizeof(student.y));
        outData += sizeof(student.y);
        memcpy(outData, &student.z, sizeof(student.z));
        outData += sizeof(student.z);
        memcpy(outData, &student.orientation, sizeof(student.orientation));
        outData += sizeof(student.orientation);
    }

    // Serialize level
    memcpy(outData, &packet.state.level, sizeof(packet.state.level));
    outData += sizeof(packet.state.level);
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