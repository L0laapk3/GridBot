#pragma once

#include <array>
#include <vector>

#include "Board.h"

class GameServer {
public:
	Board board;
	virtual void move(std::vector<unsigned long>& move) = 0;
};