#pragma once

#include "Board.h"
#include "Conn.h"


class Game {
	Board board;
	Conn conn;
	unsigned long score = 0;
	unsigned long turns = 0;
	unsigned long computationTime = 0;
};