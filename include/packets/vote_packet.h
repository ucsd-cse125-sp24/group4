#pragma once
#include <cstddef>
#include "packet.h"

enum Vote {
  UNUSED_V, // ignore 0 since it's difficult to send null
  READY,
  NOT_READY
};

class VotePacket : public Packet { // readiness vote to send to server
  public:
    Vote vote;

    size_t calculateSize() const;
    static void serialize(const VotePacket& packet, char*& outData);
    static void deserialize(const char* inData, VotePacket& packet);
};
