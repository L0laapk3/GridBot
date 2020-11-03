#pragma once

#include "IGameServer.h"
#include "Random.h"


class LocalGameServer : public IGameServer {
public:
	LocalGameServer();
	void move(std::vector<unsigned long>& move);

private:
	Random m_random;
};