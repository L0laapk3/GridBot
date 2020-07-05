
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
	while (true) {
		Conn conn("L0lBot");

		int score = 0;
		while (true) {
			conn.receiveBoard(board);



			if (0) {
				board.data = Data(std::array<uint64_t, 2>{
					0b0001'00001'00010'00101'01111'00110'10001'00001'10011'00011'00001'10010'10001,
						0b10001'10011'00010'00001'10001'00001'10100'00001'00010'10011'10001'00101'1
				});
			}

			if (0) {
				board.data = Data(std::array<uint64_t, 2>{
					0b0001'00001'00010'00001'01111'00010'10001'00001'10011'00011'00001'10010'10001,
						0b10001'10011'00010'00001'10001'00001'10100'00001'00010'10011'10001'00001'1
				});
			}

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

			//return 0;
		}

		conn.close();

		return 0;
	}
}