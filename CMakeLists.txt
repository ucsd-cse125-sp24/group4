cmake_minimum_required(VERSION 3.10)
project("CSE125 Project")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB GRAPHICS_SOURCES "${PROJECT_SOURCE_DIR}/src/*")

add_library(client OBJECT "${PROJECT_SOURCE_DIR}/networking/client.cpp")
add_library(server OBJECT "${PROJECT_SOURCE_DIR}/networking/server.cpp")

link_directories("${PROJECT_SOURCE_DIR}/lib")

add_executable(GraphicsTest ${GRAPHICS_SOURCES})
target_include_directories(GraphicsTest PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(GraphicsTest PRIVATE glfw3)

file(COPY ${PROJECT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(NetworkTest "${PROJECT_SOURCE_DIR}/networking/network_sandbox.cpp")
target_link_libraries(NetworkTest PRIVATE server client ws2_32 mswsock advapi32)

add_executable(NetworkUnitTests tests/network_test.cpp "${PROJECT_SOURCE_DIR}/networking/client.cpp" "${PROJECT_SOURCE_DIR}/networking/server.cpp")
target_link_libraries(NetworkUnitTests PRIVATE ws2_32 mswsock advapi32)

enable_testing()

add_test(NAME NetworkConnectTest COMMAND NetworkUnitTests "connect")
add_test(NAME NetworkDataTransportTest COMMAND NetworkUnitTests "data_transport")