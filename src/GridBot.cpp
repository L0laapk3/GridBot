
#include <iostream>
#include <chrono>

#include "Conn.h"
#include "Board.h"
#include "Explore.h"


Board board;



int __cdecl main(int argc, char** argv)
{
	while (true) {
		Conn conn("L0lBot");
		const auto beginTime = std::chrono::steady_clock::now();

		unsigned long score = 0;
		unsigned long moves = 0;
		long timeBudget = 10000;
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

			const auto move = findBestMove(board, timeBudget);
			if (move.size() == 0)
				break;

			unsigned long scoreIncrease = move.size() * board.valueAt(5 * move[0]);
			score += scoreIncrease;
			timeBudget += 8 * scoreIncrease;
			conn.sendMove(move);
			++moves;

			std::cout << "score: " << score << std::endl;
			std::cout << "===================================================" << std::endl;

			//return 0;
		}
		std::cout << "score: " << score << " in " << moves << " moves, " << (float)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - beginTime).count() / 100)/10 << " seconds" << std::endl;

		conn.close();

		return 0;
	}
}