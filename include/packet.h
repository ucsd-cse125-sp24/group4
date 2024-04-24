#include <string>
#include <vector>
#include "game_state.h"

struct ServerPacket {
    int id;
    std::string message;
    GameState state;
};

// Caller is responsible for creating sufficiently sized buf
inline void serialize_server_packet(ServerPacket* pkt, char* buf) {
    // Serialize id
    memcpy(buf, &(pkt->id), sizeof(pkt->id));
    buf += sizeof(pkt->id);

    // Serialize message length
    size_t message_length = pkt->message.length();
    memcpy(buf, &message_length, sizeof(message_length));
    buf += sizeof(message_length);

    // Serialize message characters
    memcpy(buf, pkt->message.c_str(), message_length);
    buf += message_length;

    //----- SERIALIZATION OF GAME STATE -----

    // Serialize number of players
    size_t numPlayers = (pkt->state.players).size();
    memcpy(buf, &numPlayers, sizeof(numPlayers));
    buf += sizeof(numPlayers);

    // Serialize each player's state including x, y, z, orientation, and score
    for (const PlayerState& player : pkt->state.players) {
        memcpy(buf, &player.x, sizeof(player.x));
        buf += sizeof(player.x);
        memcpy(buf, &player.y, sizeof(player.y));
        buf += sizeof(player.y);
        memcpy(buf, &player.z, sizeof(player.z));
        buf += sizeof(player.z);
        memcpy(buf, &player.orientation, sizeof(player.orientation));
        buf += sizeof(player.orientation);
        memcpy(buf, &player.score, sizeof(player.score));
        buf += sizeof(player.score);
    }

    // Serialize number of students
    size_t numStudents = pkt->state.students.size();
    memcpy(buf, &numStudents, sizeof(numStudents));
    buf += sizeof(numStudents);

    // Serialize each student's state including x, y, z, and orientation
    for (const StudentState& student : pkt->state.students) {
        memcpy(buf, &student.x, sizeof(student.x));
        buf += sizeof(student.x);
        memcpy(buf, &student.y, sizeof(student.y));
        buf += sizeof(student.y);
        memcpy(buf, &student.z, sizeof(student.z));
        buf += sizeof(student.z);
        memcpy(buf, &student.orientation, sizeof(student.orientation));
        buf += sizeof(student.orientation);
    }

    // Serialize level
    memcpy(buf, &pkt->state.level, sizeof(pkt->state.level));
    buf += sizeof(pkt->state.level);
}
inline static void deserialize_server_packet(ServerPacket* pkt, char* buf) {
    // Deserialize id
    memcpy(&(pkt->id), buf, sizeof(pkt->id));
    buf += sizeof(pkt->id);

    // Deserialize message length
    size_t message_length;
    memcpy(&message_length, buf, sizeof(message_length));
    buf += sizeof(message_length);

    // Deserialize message characters
    pkt->message.assign(buf, buf + message_length);
    buf += message_length;

    //----- DESERIALIZATION OF BUFFER -----

    // Deserialize number of players
    size_t numPlayers;
    memcpy(&numPlayers, buf, sizeof(numPlayers));
    buf += sizeof(numPlayers);
    pkt->state.players.clear();
    pkt->state.players.resize(numPlayers);

    for (int i = 0; i < numPlayers; i++) {
        memcpy(&pkt->state.players[i].x, buf, sizeof(pkt->state.players[i].x));
        buf += sizeof(pkt->state.players[i].x);
        memcpy(&pkt->state.players[i].y, buf, sizeof(pkt->state.players[i].y));
        buf += sizeof(pkt->state.players[i].y);
        memcpy(&pkt->state.players[i].z, buf, sizeof(pkt->state.players[i].z));
        buf += sizeof(pkt->state.players[i].z);
        memcpy(&pkt->state.players[i].orientation, buf, sizeof(pkt->state.players[i].orientation));
        buf += sizeof(pkt->state.players[i].orientation);
        memcpy(&pkt->state.players[i].score, buf, sizeof(pkt->state.players[i].score));
        buf += sizeof(pkt->state.players[i].score);
    }

    // Deserialize number of students
    size_t numStudents;
    memcpy(&numStudents, buf, sizeof(numStudents));
    buf += sizeof(numStudents);
    pkt->state.students.clear();
    pkt->state.students.resize(numStudents);

    // Deserialize each student's state including x, y, z, and orientation
    for (StudentState& student : pkt->state.students) {
        memcpy(&student.x, buf, sizeof(student.x));
        buf += sizeof(student.x);
        memcpy(&student.y, buf, sizeof(student.y));
        buf += sizeof(student.y);
        memcpy(&student.z, buf, sizeof(student.z));
        buf += sizeof(student.z);
        memcpy(&student.orientation, buf, sizeof(student.orientation));
        buf += sizeof(student.orientation);
    }

    // Deserialize level
    memcpy(&pkt->state.level, buf, sizeof(pkt->state.level));
    buf += sizeof(pkt->state.level);
}