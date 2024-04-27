#pragma once

#include "windows_socket.h"

/*
Module to connect a client to the game server
*/
class Client {
    private:
        char recvbuf[DEFAULT_BUFLEN];
        int buflen = DEFAULT_BUFLEN;
        char sendbuf[DEFAULT_BUFLEN];

    public:
        SOCKET conn_sock;

        Client();

        bool is_connected() const;

        /*
        Send data from this client to the server
        @param length: number of bytes to send
        @param data: pointer to data to send
        @return true if data was sent successfully, else false
        */
        bool sock_send(int length, const char* data);

        /*
        Receive data from the server
        @return pointer to data received
        */
        char* sock_receive();

        /*
        Close this client's connection to the server
        */
        void close_conn();
};