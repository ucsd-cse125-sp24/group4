#include "../include/packets/vote_packet.h"
#include <cstring>

size_t VotePacket::calculateSize() const {
    return sizeof(PacketType) + sizeof(Vote);
}

// Ensure outData is large enough to store all the data that is intended to be serialized.
void VotePacket::serialize(const VotePacket& packet, char*& outData) {
    char* temp = outData;

    // serialize packet type
    PacketType type = VOTE;
    memcpy(temp, &type, sizeof(type));
    temp += sizeof(type);
    
    // Serialize state
    Vote vote = packet.vote;
    memcpy(temp, &vote, sizeof(vote));
}

void VotePacket::deserialize(const char* inData, VotePacket& packet) {
    // skip packet type; we know it's SERVER_HEARTBEAT if we're calling this
    const char* temp = inData + sizeof(PacketType);

    // Deserialize vote
    Vote vote;
    memcpy(&vote, temp, sizeof(vote));

    packet.vote = vote;
}
