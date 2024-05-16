#include <iostream>
#include <cassert>
#include "../include/packets/vote_packet.h"

int main()
{
    VotePacket vote;
    // just test all states are communicated correctly since there aren't a ton
    Vote votes[] = {READY, NOT_READY};

    size_t bufferSize = vote.calculateSize();
    char *buffer = new char[bufferSize];

    for (Vote to_check : votes) {
        vote.vote = to_check;
    }
    VotePacket::serialize(vote, buffer); 

    VotePacket result;
    VotePacket::deserialize(buffer, result);

    assert(vote.vote == result.vote);

    return 0;
}
