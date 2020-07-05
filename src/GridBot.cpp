
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

		if (0) { 
			board.data = Data(std::array<uint64_t, 2>{
				0b0101'00110'00101'00110'00101'00110'00101'00110'00101'00110'00101'00110'00101,
					0b01111'01111'00010'00110'00101'10001'00001'00110'00101'00110'00101'00110'0
			});
			board.eval();
			return 0;
		}

		board.print();

		const auto move = findBestMove(board);
		if (move.size() == 0)
			break;

		score += move.size() * board.valueAt(5 * move[0]);
		conn.sendMove(move);

		std::cout << "===================================================" << std::endl;
		std::cout << "score: " << score << std::endl;
	}

	conn.close();
}