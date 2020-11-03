#pragma once

#include <array>
#include <vector>

class IGameServer {
public:
	std::array<unsigned long, 25> data;

	virtual void move(std::vector<unsigned long>& move) = 0;
};