cmake_minimum_required(VERSION 3.10)

project("CSE125 Project")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall)

add_executable(NetworkTest networking/network_sandbox.cpp networking/client.cpp networking/server.cpp)

target_link_libraries(NetworkTest ws2_32 mswsock advapi32)

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