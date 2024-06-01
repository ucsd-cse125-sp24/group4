#pragma once

#include "windows_socket.h"

/*
Module to connect a client to the game server
*/
class Client {
    private:
        char recvbuf[CLIENT_RECV_BUFLEN];
        int buflen = CLIENT_RECV_BUFLEN;
        char sendbuf[SERVER_RECV_BUFLEN];
        char addr[16]; // max len 16: xxx.xxx.xxx.xxx\0

    public:
        SOCKET conn_sock;

        Client(const char* addr = "127.0.0.1", size_t len = 10);

        SOCKET connect_to_server();

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