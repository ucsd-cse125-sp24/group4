cmake_minimum_required(VERSION 3.10)
project("CSE125 Project")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB GRAPHICS_SOURCES graphics/*)

add_library(client OBJECT networking/client.cpp)
add_library(server OBJECT networking/server.cpp)

link_directories(lib)

# graphics client contains main for graphics; don't duplicate for other executables
add_executable(GraphicsTest ${GRAPHICS_SOURCES} graphics_client.cpp)
target_include_directories(GraphicsTest PRIVATE include)
target_link_libraries(GraphicsTest PRIVATE glfw3)

file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(server_core game/game_logic_pg.cpp game/server_core.cpp packets/game_state_packet.cpp packets/input_packet.cpp game/game_state.cpp)
target_include_directories(server_core PRIVATE include)
target_link_libraries(server_core PRIVATE server client ws2_32)

add_executable(client_core game/game_logic_client.cpp game/client_core.cpp packets/game_state_packet.cpp packets/input_packet.cpp game/game_state.cpp ${GRAPHICS_SOURCES})
target_include_directories(client_core PRIVATE include)
target_link_libraries(client_core PRIVATE server client ws2_32 glfw3)


# ----- Testing executables and tests below -----
add_executable(NetworkUnitTests tests/network_test.cpp networking/client.cpp networking/server.cpp)
target_link_libraries(NetworkUnitTests PRIVATE ws2_32 mswsock advapi32)

enable_testing()

add_test(NAME NetworkConnectTest COMMAND NetworkUnitTests "connect")
add_test(NAME NetworkDataTransportTest COMMAND NetworkUnitTests "data_transport")

# ---- Packets test ----
add_executable(GameStatePacketTest tests/game_state_packet_test.cpp packets/game_state_packet.cpp)

add_executable(InputPacketTest tests/input_packet_test.cpp packets/input_packet.cpp)

add_test(NAME GameStatePacketTest COMMAND GameStatePacketTest)
add_test(NAME InputPacketTest COMMAND InputPacketTest)

# ---- NPC movements test ----
add_executable(NPCMovementsTest tests/npc_movements_test.cpp game/game_state.cpp)

add_test(NAME NPCMovementsTest COMMAND NPCMovementsTest)

