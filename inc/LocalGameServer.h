#pragma once

#include "GameServer.h"
#include "Random.h"


class LocalGameServer : public GameServer {
public:
	LocalGameServer();
	void move(std::vector<unsigned long>& move);

private:
	Random m_random;

	std::array<unsigned long, 25> data;
};