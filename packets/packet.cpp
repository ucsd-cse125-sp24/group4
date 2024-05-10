#include <cstring>

#include "../include/packets/packet.h"

void Packet::signify_type(const PacketType type) {
    this->packet_type = type;
}

PacketType Packet::get_packet_type(const char* packet_serial) {
    PacketType type;
    memcpy(&type, packet_serial, sizeof(type));
    return type;
}
