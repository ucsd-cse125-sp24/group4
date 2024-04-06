/*
Code adapted from https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
*/
#include "client.h"

Client::Client() {
    struct addrinfo* result = NULL, *ptr = NULL, hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    int iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
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
            return;
        }

        // Connect to server.
        iResult = connect(conn, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(conn);
            conn = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);

    if (conn == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        return;
    }

    this->conn_sock = conn;
}

bool Client::sock_send(int length, const char* data) {
    int iResult = send(this->conn_sock, data, length, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(this->conn_sock);
        return false;
    }

    printf("Bytes Sent: %d\n", iResult);
    return true;
}

char* Client::sock_receive(SOCKET server_conn) {
    int iResult = recv(this->conn_sock, this->recvbuf, this->buflen, 0);
    if ( iResult > 0 ) {
        printf("Bytes received: %d\n", iResult);
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
}