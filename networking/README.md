# Networking Module
This module contains files for networking via WinSock2. Tutorials are available at https://learn.microsoft.com/en-us/windows/win32/winsock/

### Files
The following files in this directory are necessary for proper networking:

`client.cpp`: client class implementation

`client.h`: client specification

`server.cpp`: server class implementation

`server.h`: server specification

`windows_socket.h`: boilerplate code needed for both winsock client and server


Additionally, `network_sandbox.cpp` is a sandboxing/testing file for testing client/server communications. This file can be compiled from the project's base directory via `make NetworkTest` and run via `./networking/network_test`. Further explanation of this testfile should be kept documented within that file.