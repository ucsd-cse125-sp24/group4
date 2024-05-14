#include "../include/packets/game_state_packet.h"
#include <cstring>

size_t GameStatePacket::calculateSize() const
{
    size_t totalSize = 0;

    totalSize += sizeof(PacketType); // store packet type

    totalSize += sizeof(size_t);  // To store the number of players
    totalSize += state.players.size() * (sizeof(float) * 16 + sizeof(int)); // Each player's size: 16 floats for world matrix, 1 int for score

    // To store the number of students
    totalSize += sizeof(size_t);
    // Each student's size: 16 floats for world matrix, 1 enum for direction, 1 float for distance moved
    totalSize += state.students.size() * (sizeof(float) * 16 + sizeof(StudentState::Direction) + sizeof(float));

    // Add size of level
    totalSize += sizeof(state.level);

    return totalSize;
}

// Ensure outData is large enough to store all the data that is intended to be serialized.
void GameStatePacket::serialize(const GameStatePacket& packet, char*& outData) {
    char* temp = outData;
    
    // serialize packet type
    PacketType type = GAME_STATE;
    memcpy(temp, &type, sizeof(type));
    temp += sizeof(type);

    // Serialize number of players
    size_t numPlayers = packet.state.players.size();
    memcpy(temp, &numPlayers, sizeof(numPlayers));
    temp += sizeof(numPlayers);

    // Serialize each player's state including 16 floats of world matrix, and score
    for (const PlayerState &player : packet.state.players)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                memcpy(temp, &player.world[i][j], sizeof(player.world[i][j]));
                temp += sizeof(player.world[i][j]);
            }
        }

        memcpy(temp, &player.score, sizeof(player.score));
        temp += sizeof(player.score);
    }

    // Serialize number of students
    size_t numStudents = packet.state.students.size();
    memcpy(temp, &numStudents, sizeof(numStudents));
    temp += sizeof(numStudents);

    // Serialize each student's state including 16 floats of world matrix
    for (const StudentState &student : packet.state.students)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                memcpy(temp, &student.world[i][j], sizeof(student.world[i][j]));
                temp += sizeof(student.world[i][j]);
            }
        }

        memcpy(temp, &student.currentDir, sizeof(student.currentDir));
        temp += sizeof(student.currentDir);

        memcpy(temp, &student.distanceMoved, sizeof(student.distanceMoved));
        temp += sizeof(student.distanceMoved);
    }

    // Serialize level
    memcpy(temp, &packet.state.level, sizeof(packet.state.level));
    temp += sizeof(packet.state.level);
}

void GameStatePacket::deserialize(const char* inData, GameStatePacket& packet) {
    // skip packet type; we know it's GAME_STATE if we're calling this
    inData += sizeof(PacketType);

    // Deserialize number of players
    size_t numPlayers;
    memcpy(&numPlayers, inData, sizeof(numPlayers));
    inData += sizeof(numPlayers);
    packet.state.players.clear();
    packet.state.players.resize(numPlayers);

    // Deserialize each player's state including 16 floats for world matrix and score
    for (PlayerState &player : packet.state.players)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                memcpy(&player.world[i][j], inData, sizeof(player.world[i][j]));
                inData += sizeof(player.world[i][j]);
            }
        }

        memcpy(&player.score, inData, sizeof(player.score));
        inData += sizeof(player.score);
    }

    // Deserialize number of students
    size_t numStudents;
    memcpy(&numStudents, inData, sizeof(numStudents));
    inData += sizeof(numStudents);
    packet.state.students.clear();
    packet.state.students.resize(numStudents);

    // Deserialize each student's state including 16 floats for world matrix
    for (StudentState &student : packet.state.students)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                memcpy(&student.world[i][j], inData, sizeof(student.world[i][j]));
                inData += sizeof(student.world[i][j]);
            }
        }

        memcpy(&student.currentDir, inData, sizeof(student.currentDir));
        inData += sizeof(student.currentDir);

        memcpy(&student.distanceMoved, inData, sizeof(student.distanceMoved));
        inData += sizeof(student.distanceMoved);
    }

    // Deserialize level
    memcpy(&packet.state.level, inData, sizeof(packet.state.level));
    inData += sizeof(packet.state.level);
}