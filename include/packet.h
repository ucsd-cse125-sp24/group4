#include <string>
#include <vector>

class Packet {
    public:
        struct ServerPacket {
            int id;
            std::vector<int> coor;
            std::string message;
        };

        void serialize_server_packet(ServerPacket* pkt, char* buf);
        void deserialize_server_packet(ServerPacket* pkt, char* buf);
};