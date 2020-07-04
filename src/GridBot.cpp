
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

		if (false)
			board.data = Data(std::array<uint64_t, 2>{
				0b0101'00110'00101'00110'00101'00110'00101'00110'00101'00110'00101'00110'00101,
					0b00010'01111'10001'00110'00101'10001'00001'00110'00101'00110'00101'00110'0
			});

		board.print();

		const auto compressedMove = findBestMove(board);
		if (compressedMove == 0)
			break;
		board.print();

		const auto move = board.decompressMove(compressedMove);
		score += move.size() * board.valueAt(5 * move[0]);
		conn.sendMove(move);

		std::cout << "===================================================" << std::endl;
		std::cout << "score: " << score << std::endl;
	}

	conn.close();
}