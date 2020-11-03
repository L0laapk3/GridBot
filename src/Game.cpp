
#include "Game.h"

#include <string>

#include "GameManager.h"
#include <Explore.h>



Game::Game() { }


bool Game::move() {

	const auto move = findBestMove(board, 100);
	if (move.size() == 0)
		return false;

	unsigned long scoreIncrease = move.size() * board.valueAt(5 * move[0]);
	score += scoreIncrease;
	++moves;

	conn.sendMove(move);

	return true;
}