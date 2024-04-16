/*
Code adapted from https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
*/
#include "client.h"
#include <windows.h>

Client::Client() {
    struct addrinfo* result = NULL, *ptr = NULL, hints;
    this->conn_sock = INVALID_SOCKET;

    WSADATA wsaData;

    int wresult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (wresult != 0) {
        printf("WSAStartup failed: %d\n", wresult);
        return;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    int iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return;
    }

    SOCKET conn = INVALID_SOCKET;
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL; ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        conn = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (conn == INVALID_SOCKET) {
            printf("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            return;
        }

        // Connect to server.
        iResult = connect(conn, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(conn);
            conn = INVALID_SOCKET;
            continue;
        }
        printf("successfully connected.\n");
        break;
    }
    freeaddrinfo(result);

    if (conn == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return;
    }

    this->conn_sock = conn;
    Sleep(100*CONNECT_TIMEOUT);
}

bool Client::sock_send(int length, const char* data) {
    int iResult = send(this->conn_sock, data, length, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(this->conn_sock);
        return false;
    }

    printf("Bytes sent from client: %d\n", iResult);
    return true;
}

char* Client::sock_receive() {
    int iResult = recv(this->conn_sock, this->recvbuf, this->buflen, 0);
    if ( iResult > 0 ) {
        printf("Bytes received from server: %d\n", iResult);
        return this->recvbuf;
    } else if ( iResult == 0 )
        printf("Connection closed\n");
    else
        printf("recv failed with error: %d\n", WSAGetLastError());
    return NULL;
}

void Client::close_conn() {
    int iResult = shutdown(this->conn_sock, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(this->conn_sock);
    }
    WSACleanup();
}