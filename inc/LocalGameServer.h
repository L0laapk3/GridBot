#pragma once

#include "IGameServer.h"
#include "Random.h"

constexpr uint64_t SEED = 1;


class LocalGameServer : public IGameServer {
public:
	LocalGameServer();
	void move(std::vector<unsigned long>& move);

private:
	Random m_random = Random(SEED);
};