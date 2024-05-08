cmake_minimum_required(VERSION 3.10)
project("CSE125 Project")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB GRAPHICS_SOURCES graphics/*)
file(GLOB PACKET_SOURCES packets/*)

add_library(client OBJECT networking/client.cpp)
add_library(server OBJECT networking/server.cpp)

link_directories(lib)

# graphics client contains main for graphics; don't duplicate for other executables
add_executable(GraphicsTest ${GRAPHICS_SOURCES} graphics_client.cpp)
target_include_directories(GraphicsTest PRIVATE include)
target_link_libraries(GraphicsTest PRIVATE glfw3)

file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(server_core game/game_logic_pg.cpp game/server_core.cpp ${PACKET_SOURCES})
target_include_directories(server_core PRIVATE include)
target_link_libraries(server_core PRIVATE server client ws2_32)

add_executable(client_core game/game_logic_client.cpp game/client_core.cpp ${PACKET_SOURCES} ${GRAPHICS_SOURCES})
target_include_directories(client_core PRIVATE include)
target_link_libraries(client_core PRIVATE server client ws2_32 glfw3)


# ----- Testing executables and tests below -----
add_executable(NetworkUnitTests tests/network_test.cpp)
target_link_libraries(NetworkUnitTests PRIVATE server client ws2_32 mswsock advapi32)

add_executable(ServerCoreTests tests/servercore_test.cpp game/server_core.cpp ${PACKET_SOURCES})
target_link_libraries(ServerCoreTests PRIVATE server client ws2_32)

#add_executable(ClientCoreTests tests/clientcore_test.cpp game/client_core.cpp ${PACKET_SOURCES} ${GRAPHICS_SOURCES})
#target_link_libraries(ClientCoreTests PRIVATE server client ws2_32 glfw3)

enable_testing()

add_test(NAME NetworkConnectTest COMMAND NetworkUnitTests "connect")
add_test(NAME NetworkDataTransportTest COMMAND NetworkUnitTests "data_transport")

add_test(NAME ServerCoreListenAcceptTest COMMAND ServerCoreTests "listen_accept")
add_test(NAME ServerCoreReceiveTest COMMAND ServerCoreTests "receive")
add_test(NAME ServerCoreUpdateTest COMMAND ServerCoreTests "update")
add_test(NAME ServerCoreSendTest COMMAND ServerCoreTests "send")
add_test(NAME ServerCoreShutdownTest COMMAND ServerCoreTests "shutdown")

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

add_test(NAME GameStatePacketTest COMMAND GameStatePacketTest)
add_test(NAME InputPacketTest COMMAND InputPacketTest)

# ---- NPC movements test ----
add_executable(NPCMovementsTest tests/npc_movements_test.cpp game/game_state.cpp)

add_test(NAME NPCMovementsTest COMMAND NPCMovementsTest)

