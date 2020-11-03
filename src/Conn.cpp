#include "Conn.h"

#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <chrono>
#include <thread>
#include <iostream>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512


Conn::Conn() {
    WSADATA wsaData;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        exit(1);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("35.193.192.221", "32123", &hints, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        exit(2);
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            exit(3);
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        std::cout << "Unable to connect to server!" << std::endl;
        WSACleanup();
        exit(4);
    }

    sendData("\"" + (std::string)NAME + "\"\n");
}




void Conn::sendData(const std::string& msg) {

    const auto sendbuf = msg.c_str();
    const int len = strlen(sendbuf);
    // Send an initial buffer
    int iResult = send(ConnectSocket, sendbuf, len, 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        exit(5);
    }
}

Conn::~Conn() {
    // shutdown the connection since no more data will be sent
    int iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        exit(6);
    }
}


void Conn::receive(Board& board) {

    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    std::string s = "";

    bool enter = false;
    while (!enter) {
        size_t bytes = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (bytes == 0)
            exit(1);
        for (int i = 0; i < bytes; i++) {
            if (recvbuf[i] == '\n') {
                assert(i == bytes - 1);
                enter = true;
                break;
            }
            s += recvbuf[i];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    //std::cout << s << std::endl;

    std::array<unsigned long, 25> result;

    int i = 0;
    int j = 1;
    uint64_t value = 0;
    while (i < 25) {
        if (isdigit(s[j]))
            value = 10ULL * value + ((uint64_t)s[j] - 48ULL);
        else {
            result[i] = value;
            i++;
            value = 0;
        }
        ++j;
    }

    board.set(result);
};


void Conn::sendMove(const std::vector<unsigned long> move) {
    std::string s = "[" + std::to_string(move[0]);
    for (int i = 1; i < move.size(); i++) {
        s += "," + std::to_string(move[i]);
    }
    s += "]\n";
    //std::cout << s;
    sendData(s);
}