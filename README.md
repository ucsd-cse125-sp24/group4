Group 4's project for CSE 125

### How to run things ... for now
#### CMake version
Game (servercore):

cd build
cmake --build . --clean-first -t game
./Debug/game.exe

Graphics:

cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
./GraphicsTest

Network:



#### g++ version
Game (servercore):

g++ game/game_logic_pg.cpp game/server_core.cpp networking/client.cpp networking/server.cpp -o game.exe -Iinclude -lws2_32

./game

Game (client):
g++ game/game_logic_client.cpp game/server_core.cpp networking/client.cpp networking/server.cpp -o game_client.exe -Iinclude -lws2_32

./game_client

Network:

g++ network_sandbox.cpp server.cpp client.cpp -o network.exe -lws2_32

./network

Graphics:
