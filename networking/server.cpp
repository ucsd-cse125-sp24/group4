/*
Code adapted from https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code
*/
#include "../include/server.h"
#include <algorithm>
#include <vector>
#include <windows.h>

Server::Server() {
    struct addrinfo *result = NULL, hints;

    WSADATA wsaData;

    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return;
    }
    
    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return;
    }

    // Create a SOCKET for the server to listen for client connections
    this->listen_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (this->listen_sock == INVALID_SOCKET) {
        printf("Error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    iResult = bind(this->listen_sock, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(this->listen_sock);
        WSACleanup();
        return;
    }
    freeaddrinfo(result);

    iResult = listen(this->listen_sock, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        this->sock_shutdown();
        return;
    }
}

SOCKET Server::get_client_sock(int i) {
    if (i > this->get_num_clients())
        return INVALID_SOCKET;
    return this->connections[i];
}

int Server::get_num_clients() {
    return int(this->connections.size());
}

void Server::sock_listen() {
    SOCKET client_conn = INVALID_SOCKET;
    fd_set readFdSet;
    FD_ZERO(&readFdSet);
    FD_SET(this->listen_sock, &readFdSet);
    timeval timeout;
    timeout.tv_sec = CONNECT_TIMEOUT;
    timeout.tv_usec = 0;

    if (this->get_num_clients() >= MAX_CLIENTS) {
        printf("Maximum clients reached, cannot connect more\n");
        return;
    }
    if (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0) {
        client_conn = accept(this->listen_sock, NULL, NULL);
        printf("connected %d\n", this->get_num_clients());
        this->connections.push_back(client_conn);
    }
}

bool Server::sock_send(SOCKET client_conn, int length, const char* data) {
    int iSendResult = send(client_conn, data, length, 0 );
    if (iSendResult == SOCKET_ERROR) {
        // printf("send failed with error: %d\n", WSAGetLastError());
        // this->sock_shutdown();
        
        // remove client connection from connections
        auto i = std::begin(this->connections);
        while (i != std::end(this->connections)) {
            if (*i == client_conn)
                i = this->connections.erase(i);
            else
                i++;
        }
        return false;
    }
    printf("Bytes sent from server: %d\n", iSendResult);
    return true;
}

char* Server::sock_receive(SOCKET client_conn) {
    int iResult = recv(client_conn, this->recvbuf, this->buflen, 0);
    if (iResult > 0) {
        printf("Bytes received from client: %d\n", iResult);
        return this->recvbuf;
    }
    else if (iResult == 0) {
        // printf("Nothing to receive.");
        printf("Connection closed by client.\n");
        this->close_client(client_conn); // Close the client socket
        return NULL; // return some data flag to tell servercore some client had disconnected, then clear data
    }
    else  {
        if (WSAGetLastError() == 10053) { // If client shutdown, remove client from connections.
            auto i = std::begin(this->connections);
            while (i != std::end(this->connections)) {
                if (*i == client_conn)
                    i = this->connections.erase(i);
                else
                    i++;
            }
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
        }    
    }
    return NULL;
}

void Server::close_client(SOCKET client_conn) {
    // shut down connection to client, then close corresponding socket
    shutdown(client_conn, SD_SEND);
    closesocket(client_conn);
    this->connections.erase(std::remove(this->connections.begin(),
                                        this->connections.end(),
                                        client_conn), this->connections.end());
}

void Server::sock_shutdown() {
    // shut down any client connections, then close the server's listening socket
    for (SOCKET client : this->connections) {
        this->close_client(client);
    }
    closesocket(this->listen_sock);
    WSACleanup();
}