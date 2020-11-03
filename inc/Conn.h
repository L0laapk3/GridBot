
#pragma once

#define NAME "L0lBot"

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>

#include <string>
#include <array>
#include <vector>

class Conn {
public:
	Conn();
	void receiveBoard(std::array<unsigned long, 25>& result);
	void sendMove(const std::vector<unsigned long> move);
	~Conn();

private:
	SOCKET ConnectSocket = INVALID_SOCKET;

	void sendData(const std::string& sendbuf);
};
