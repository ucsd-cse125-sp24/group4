CXX=g++
CXXFLAGS?=-Wall -pedantic -g -O0 -std=c++11
OUTFILES=network_test
NETWORK_FILES=networking/network_sandbox.cpp networking/client.cpp networking/server.cpp

all: $(OUTFILES)

#Network: networking/server.cpp networking/server.h networking/client.cpp networking/client.h networking/windows_socket.h
#	$(CXX) $(CXXFLAGS) -o networking/server networking/server.cpp -l ws2_32
#	$(CXX) $(CXXFLAGS) -o networking/client networking/client.cpp -l ws2_32 -l mswsock -l advapi32
NetworkTest: networking/network_sandbox.cpp
	$(CXX) $(CXXFLAGS) -o networking/network_test $(NETWORK_FILES) -l ws2_32 -l mswsock -l advapi32

clean:
	$(RM) $(OUTFILES) *.o