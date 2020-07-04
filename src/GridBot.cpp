
#include <iostream>

#include "Conn.h"
#include "Board.h"
#include "Explore.h"


Board board;


int count = 0;
void cbFn(const Board& board, const Move& move) {
	++count;
}

int __cdecl main(int argc, char** argv)
{
	Conn conn("L0lBot");

	int score = 0;
	while (true) {
		conn.receiveBoard(board);
		board.print();

		const auto compressedMove = findBestMove(board);
		if (compressedMove == 0)
			break;
		const auto move = board.decompressMove(compressedMove);
		score += move.size() * board.valueAt(5 * move[0]);
		conn.sendMove(move);

		std::cout << "===================================================" << std::endl;
		std::cout << "score: " << score << std::endl;

		return 1;
	}

	conn.close();
}