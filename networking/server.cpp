/*
Code adapted from https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code
*/
#include "server.h"
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

    for (int i = 0; i < int(MAX_CLIENTS); i++) { // TODO: I had problems making this a vector, but a vector would be preferable here
        this->connections[i] = INVALID_SOCKET;
    }

    this->num_connections = 0;
    this->is_running = false;
}

SOCKET Server::get_client_sock(int i) {
    if (i >= int(MAX_CLIENTS))
        return INVALID_SOCKET;
    return this->connections[i];
}

int Server::get_num_clients() {
    return this->num_connections;
}

void Server::sock_listen() {
    this->is_running = true;
    int iResult = listen(this->listen_sock, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        this->sock_shutdown();
        return;
    }

    SOCKET client_conn = INVALID_SOCKET;
    fd_set readFdSet;
    FD_ZERO(&readFdSet);
    FD_SET(this->listen_sock, &readFdSet);
    timeval timeout;
    timeout.tv_sec = CONNECT_TIMEOUT;
    timeout.tv_usec = 0;

    while (this->is_running) {
        if (this->get_num_clients() >= MAX_CLIENTS) {
            printf("Maximum clients reached, cannot connect more\n");
            return;
        }
        if (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0) {
            if (!this->is_running) { // must recheck due to possible race w/ timeout
                printf("Server is no longer running! Cannot connect to client.\n");
                return;
            }
            client_conn = accept(this->listen_sock, NULL, NULL);
            printf("connected %d\n", this->get_num_clients());
            this->connections[this->get_num_clients()] = client_conn;
            this->num_connections++;
        }
    }
}

bool Server::sock_send(SOCKET client_conn, int length, const char* data) {
    int iSendResult = send(client_conn, recvbuf, length, 0 );
    if (iSendResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        this->sock_shutdown();
        return false;
    }
    printf("Bytes sent: %d\n", iSendResult);
    return true;
}

char* Server::sock_receive(SOCKET client_conn) {
    // TODO: add check that there's something ready to receive from this client
    int iResult = recv(client_conn, this->recvbuf, this->buflen, 0);
    if (iResult > 0) {
        printf("Bytes received: %d\n", iResult);
        return this->recvbuf;
    }
    else if (iResult == 0)
        printf("Nothing to receive.");
    else  {
        printf("recv failed with error: %d\n", WSAGetLastError());
        this->sock_shutdown();
    }
    return NULL;
}

void Server::close_client(SOCKET client_conn) {
    // shut down connection to client, then close corresponding socket
    shutdown(client_conn, SD_SEND);
    closesocket(client_conn);
    this->num_connections--;
}

void Server::sock_shutdown() {
    this->is_running = false;
    // shut down any client connections, then close the server's listening socket
    for (int i = 0; i < int(MAX_CLIENTS); i++) {
        if (this->connections[i] != INVALID_SOCKET) {
            this->close_client(this->connections[i]);
            this->connections[i] = INVALID_SOCKET;
        }
    }
    closesocket(this->listen_sock);
    WSACleanup();
}