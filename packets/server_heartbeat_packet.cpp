#include "../include/packets/server_heartbeat_packet.h"
#include <cstring>

size_t ServerHeartbeatPacket::calculateSize() const {
    return sizeof(PacketType) + sizeof(ServerState);
}

// Ensure outData is large enough to store all the data that is intended to be serialized.
void ServerHeartbeatPacket::serialize(const ServerHeartbeatPacket& hb, char*& outData) {
    char* temp = outData;

    // serialize packet type
    PacketType type = SERVER_HEARTBEAT;
    memcpy(temp, &type, sizeof(type));
    temp += sizeof(type);
    
    // Serialize state
    ServerState state = hb.state;
    memcpy(temp, &state, sizeof(state));
}

void ServerHeartbeatPacket::deserialize(const char* inData, ServerHeartbeatPacket& hb) {
    // skip packet type; we know it's SERVER_HEARTBEAT if we're calling this
    const char* temp = inData + sizeof(PacketType);

    // Deserialize state
    ServerState state;
    memcpy(&state, temp, sizeof(state));

    hb.state = state;
}
