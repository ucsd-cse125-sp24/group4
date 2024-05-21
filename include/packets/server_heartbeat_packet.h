#pragma once
#include <cstddef>
#include "packet.h"

enum ServerState {
  UNUSED_SS, // ignore 0 since it's difficult to send null
  LOBBY,
  MAIN_LOOP,
  END_WIN,
  END_LOSE
};

class ServerHeartbeatPacket : public Packet { // heartbeat to send to clients to confirm state etc.
    public:
        ServerState state;

        size_t calculateSize() const;
        static void serialize(const ServerHeartbeatPacket& hb, char*& outData);
        static void deserialize(const char* inData, ServerHeartbeatPacket& hb);
};
