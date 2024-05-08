#include <iostream>
#include <cassert>
#include "../include/packets/server_heartbeat_packet.h"

int main()
{
    ServerHeartbeatPacket hb;
    // just test all states are communicated correctly since there aren't a ton
    ServerState states[] = {LOBBY, MAIN_LOOP, END_LOSE, END_WIN};

    size_t bufferSize = hb.calculateSize();
    char *buffer = new char[bufferSize];

    for (ServerState state : states) {
        hb.state = state;
    }
    ServerHeartbeatPacket::serialize(hb, buffer); 

    ServerHeartbeatPacket result;
    ServerHeartbeatPacket::deserialize(buffer, result);

    assert(hb.state == result.state);

    return 0;
}
