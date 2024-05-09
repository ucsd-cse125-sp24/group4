/*
Socket setup common to both server and client

also a sandbox for now

Code sourced from https://learn.microsoft.com/en-us/windows/win32/winsock/
Specifically, "Getting started with Winsock" and "Initializing Winsock"
*/
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
#define CLIENT_RECV_BUFLEN 1024
#define SERVER_RECV_BUFLEN 64
#define CONNECT_TIMEOUT 1