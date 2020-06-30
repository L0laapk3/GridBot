#pragma once

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>

#include <string>

#include "Board.h"

class Conn {
public:
	Conn(const std::string name);
	void receiveBoard(Board& board);
	void sendMove(const std::vector<int> move);
	void close();

private:
	SOCKET ConnectSocket = INVALID_SOCKET;

	void sendData(const std::string& sendbuf);
};