#include <string>
#include <vector>

struct ServerPacket {
    int id;
    std::vector<int> coor;
    std::string message;
};

inline void serialize_server_packet(ServerPacket* pkt, char* buf) {
    size_t offset = 0;

    // Serialize id
    memcpy(buf + offset, &(pkt->id), sizeof(pkt->id));
    offset += sizeof(pkt->id);

    // Serialize coordinate vector size
    size_t coor_size = pkt->coor.size();
    memcpy(buf + offset, &coor_size, sizeof(coor_size));
    offset += sizeof(coor_size);

    // Serialize coordinates
    for (const auto& coord : pkt->coor) {
        memcpy(buf + offset, &coord, sizeof(coord));
        offset += sizeof(coord);
    }

    // Serialize message length
    size_t message_length = pkt->message.length();
    memcpy(buf + offset, &message_length, sizeof(message_length));
    offset += sizeof(message_length);

    // Serialize message characters
    memcpy(buf + offset, pkt->message.c_str(), message_length);
}
inline static void deserialize_server_packet(ServerPacket* pkt, char* buf) {
    size_t offset = 0;

    // Deserialize id
    memcpy(&(pkt->id), buf + offset, sizeof(pkt->id));
    offset += sizeof(pkt->id);

    // Deserialize coordinate vector size
    size_t coor_size;
    memcpy(&coor_size, buf + offset, sizeof(coor_size));
    offset += sizeof(coor_size);

    // Deserialize coordinates
    pkt->coor.clear(); // Clear existing coordinates
    for (size_t i = 0; i < coor_size; ++i) {
        int coord;
        memcpy(&coord, buf + offset, sizeof(coord));
        pkt->coor.push_back(coord);
        offset += sizeof(coord);
    }

    // Deserialize message length
    size_t message_length;
    memcpy(&message_length, buf + offset, sizeof(message_length));
    offset += sizeof(message_length);

    // Deserialize message characters
    pkt->message.assign(buf + offset, buf + offset + message_length);
}