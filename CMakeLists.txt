cmake_minimum_required(VERSION 3.10)
project("CSE125 Project")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB GRAPHICS_SOURCES "${PROJECT_SOURCE_DIR}/src/*")

add_library(client OBJECT "${PROJECT_SOURCE_DIR}/networking/client.cpp")
add_library(server OBJECT "${PROJECT_SOURCE_DIR}/networking/server.cpp")
add_library(packet OBJECT "${PROJECT_SOURCE_DIR}/game/packet.cpp")

link_directories("${PROJECT_SOURCE_DIR}/lib")

# graphics client contains main for graphics; don't duplicate for other executables
add_executable(GraphicsTest ${GRAPHICS_SOURCES} "${PROJECT_SOURCE_DIR}/graphics_client.cpp")
target_include_directories(GraphicsTest PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(GraphicsTest PRIVATE glfw3)

file(COPY ${PROJECT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(server_core "${PROJECT_SOURCE_DIR}/game/game_logic_pg.cpp" "${PROJECT_SOURCE_DIR}/game/server_core.cpp")
target_include_directories(server_core PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(server_core PRIVATE server client packet ws2_32)

add_executable(client_core "${PROJECT_SOURCE_DIR}/game/game_logic_client.cpp" "${PROJECT_SOURCE_DIR}/game/server_core.cpp" ${GRAPHICS_SOURCES})
target_include_directories(client_core PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(client_core PRIVATE server client packet ws2_32 glfw3)


# ----- Testing executables and tests below -----
add_executable(NetworkUnitTests tests/network_test.cpp "${PROJECT_SOURCE_DIR}/networking/client.cpp" "${PROJECT_SOURCE_DIR}/networking/server.cpp")
target_link_libraries(NetworkUnitTests PRIVATE ws2_32 mswsock advapi32)

enable_testing()

add_test(NAME NetworkConnectTest COMMAND NetworkUnitTests "connect")
add_test(NAME NetworkDataTransportTest COMMAND NetworkUnitTests "data_transport")

# ---- Packets test ----
add_executable(GameStatePacketTest tests/game_state_packet_test.cpp "${PROJECT_SOURCE_DIR}/packets/game_state_packet.cpp")
add_executable(InputPacketTest tests/input_packet_test.cpp "${PROJECT_SOURCE_DIR}/packets/input_packet.cpp")

add_test(NAME GameStatePacketTest COMMAND GameStatePacketTest)
add_test(NAME InputPacketTest COMMAND InputPacketTest)