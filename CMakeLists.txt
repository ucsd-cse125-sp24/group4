cmake_minimum_required(VERSION 3.10)

project("CSE125 Project")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(client OBJECT networking/client.cpp)
add_library(server OBJECT networking/server.cpp)

add_executable(NetworkTest networking/network_sandbox.cpp)
target_link_libraries(NetworkTest server client ws2_32 mswsock advapi32)

# testbench
add_executable(NetworkUnitTests tests/network_test.cpp networking/client.cpp networking/server.cpp)
target_link_libraries(NetworkUnitTests ws2_32 mswsock advapi32)

enable_testing()

add_test(
    NAME NetworkConnectTest
    COMMAND $<TARGET_FILE:NetworkUnitTests> "connect"
)
add_test(
    NAME NetworkDataTransportTest
    COMMAND $<TARGET_FILE:NetworkUnitTests> "data_transport"
)