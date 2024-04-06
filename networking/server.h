#pragma once

#include "windows_socket.h"
#include <vector>

#define MAX_CLIENTS 8

/*
Module to run the game server
*/
class Server {
    private:
        SOCKET listen_sock;
        char sendbuf[DEFAULT_BUFLEN];
        char recvbuf[DEFAULT_BUFLEN];
        int buflen = DEFAULT_BUFLEN;
        SOCKET connections[MAX_CLIENTS];

    public:
        Server();

        /*
        Get the socket for the ith client connected to this server
        */
        SOCKET get_client_sock(int i);
        
        /*
        Instruct server to listen for new connections
        */
        void sock_listen();

        /*
        Send data from the server to a client
        @param client_conn: Server-side socket for connection to client
        @param length: number of bytes to send
        @param data: pointer to data to send
        @return true if data is sent successfully, else false
        */
        bool sock_send(SOCKET client_conn, int length, const char* data);

        /*
        Receive data from a client
        @param client_conn: Server-side socket for connection to client
        @return pointer to data received
        */
        char* sock_receive(SOCKET client_conn);

        /*
        Close the connection to a client
        @param client_conn: Server-side socker for connection to client
        */
        void close_client(SOCKET client_conn);

        /*
        Close all connections to the server and shut it down
        */
        void sock_shutdown();
};