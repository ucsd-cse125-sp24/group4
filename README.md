# Group 4's project for CSE 125

### Using CMake
```
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

#### Game
server
```
./server_core
```
client
```
./client_core
```

#### Network:
```
./NetworkUnitTests connect
./NetworkUnitTests data_transport
```
or
```
ctest -C Debug
```

#### Graphics:
```
./GraphicsTest
```

### Using g++
#### Game
server
```
g++ game/game_logic_pg.cpp game/server_core.cpp networking/client.cpp networking/server.cpp -o game.exe -Iinclude -lws2_32
./game
```

client
```
g++ game/game_logic_client.cpp game/server_core.cpp networking/client.cpp networking/server.cpp -o game_client.exe -Iinclude -lws2_32
./game_client
```

#### Network:
```
g++ networking/network_sandbox.cpp networking/server.cpp networking/client.cpp -o network.exe -lws2_32
./network
```

#### Graphics:
```
g++ -std=c++11 -Iinclude -Llib -o GraphicsTest ./src/* -lglfw3 -lopengl32 -lgdi32 -lpthread
./GraphicsTest
```
