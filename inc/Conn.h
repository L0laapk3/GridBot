
#pragma once

#define NAME "L0lBot"

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>

#include <string>
#include <array>
#include <vector>

#include "Board.h"

class Conn {
public:
	Conn();
	void receive(Board& board);
	void sendMove(const std::vector<unsigned long> move);
	~Conn();

private:
	SOCKET ConnectSocket = INVALID_SOCKET;

	void sendData(const std::string& sendbuf);
};
