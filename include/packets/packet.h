#pragma once

enum PacketType {
    UNUSED_P, // problems sending null input (enum evaluates to 0), so just skip it
    PLAYER_INPUT,
    GAME_STATE,
    SERVER_HEARTBEAT
};

/*
Base class for Packets sent between server and client cores
*/
class Packet {
    private:
        PacketType packet_type;

    public:
        size_t calculateSize() const;                           // abstract func to calculate packet size
        void serialize(const Packet& input, char*& outData);    // abstract func to serialize packet to str
        void deserialize(const char* inData, Packet& input);    // abstract func to deserialize packet from str
        
        void signify_type(const PacketType type);                           // assign specified type to Packet
        static PacketType get_packet_type(const char* packet_serial);       // retrieve packet's type from serial
};
