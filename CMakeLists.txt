cmake_minimum_required(VERSION 3.10)
project("CSE125 Project")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB GRAPHICS_SOURCES graphics/*)
file(GLOB PACKET_SOURCES packets/*)

add_library(client OBJECT networking/client.cpp)
add_library(server OBJECT networking/server.cpp)

link_directories(lib)

file(COPY include/assimp/bin/libassimp-5.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY art DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(server_core game/game_logic_pg.cpp game/server_core.cpp ${PACKET_SOURCES})
target_include_directories(server_core PRIVATE include)
target_link_libraries(server_core PRIVATE server client ws2_32)

add_executable(client_core game/game_logic_client.cpp game/client_core.cpp ${PACKET_SOURCES} ${GRAPHICS_SOURCES})
target_include_directories(client_core PRIVATE include include/assimp/include)
target_link_libraries(client_core PRIVATE server client ws2_32 winmm glfw3 assimp zlibstatic)

# ----- Testing executables and tests below -----
enable_testing()

# ---- Network tests ----
add_executable(NetworkUnitTests tests/network_test.cpp)
target_link_libraries(NetworkUnitTests PRIVATE server client ws2_32 mswsock advapi32)

add_test(NAME NetworkConnectTest COMMAND NetworkUnitTests "connect")
add_test(NAME NetworkDataTransportTest COMMAND NetworkUnitTests "data_transport")

# ---- Core tests ----
add_executable(ServerCoreTests tests/servercore_test.cpp game/server_core.cpp ${PACKET_SOURCES})
target_link_libraries(ServerCoreTests PRIVATE server client ws2_32)

#add_executable(ClientCoreTests tests/clientcore_test.cpp game/client_core.cpp ${PACKET_SOURCES} ${GRAPHICS_SOURCES})
#target_link_libraries(ClientCoreTests PRIVATE server client ws2_32 glfw3)

add_test(NAME ServerCoreListenAcceptTest COMMAND ServerCoreTests "listen_accept")
add_test(NAME ServerCoreReceiveTest COMMAND ServerCoreTests "receive")
add_test(NAME ServerCoreUpdateTest COMMAND ServerCoreTests "update")
add_test(NAME ServerCoreSendTest COMMAND ServerCoreTests "send")
add_test(NAME ServerCoreShutdownTest COMMAND ServerCoreTests "shutdown")

# // commented out the below bc of difficulties unit-testing w graphics,
# // not sure its worth modularizing to test separately from graphics so leave like this for now
# add_test(NAME ClientCoreInitializetTest COMMAND ClientCoreTests "initialize")
# add_test(NAME ClientCoreReceiveTest COMMAND ClientCoreTests "receive")
# add_test(NAME ClientCoreProcessTest COMMAND ClientCoreTests "process")
# add_test(NAME ClientCoreRenderTest COMMAND ClientCoreTests "render")
# add_test(NAME ClientCoreSendTest COMMAND ClientCoreTests "send")
# add_test(NAME ClientCoreShutdownTest COMMAND ClientCoreTests "shutdown")

# ---- Packets test ----
add_executable(GameStatePacketTest tests/game_state_packet_test.cpp packets/game_state_packet.cpp)
target_include_directories(GameStatePacketTest PRIVATE include)

add_executable(InputPacketTest tests/input_packet_test.cpp packets/input_packet.cpp)

add_executable(ServerHeartbeatPacketTest tests/server_heartbeat_packet_test.cpp packets/server_heartbeat_packet.cpp)

add_executable(VotePacketTest tests/vote_packet_test.cpp packets/vote_packet.cpp)

add_test(NAME GameStatePacketTest COMMAND GameStatePacketTest)
add_test(NAME InputPacketTest COMMAND InputPacketTest)
add_test(NAME ServerHeartbeatPacketTest COMMAND ServerHeartbeatPacketTest)
add_test(NAME VotePacketTest COMMAND VotePacketTest)

# ---- NPC movements test ----
add_executable(NPCMovementsTest tests/npc_movements_test.cpp game/game_state.cpp)

add_test(NAME NPCMovementsTest COMMAND NPCMovementsTest)

