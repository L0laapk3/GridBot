
#include "Board.h"

#include <iostream>


// basic eval that counts how many connections each cell has
uint32_t Board::eval() const {
	uint32_t score = 0;

	constexpr Data randomMask = repeat(0b01111);
	Data randoms = compareCells(data, randomMask);

	Data d123Check = ~(((data | data >> 1) & data >> 4) | (data & data >> 1) | data >> 2 | data >> 3);
	Data d123 = d123Check & d123Check >> 1 & d123Check >> 2 & d123Check >> 3 & d123Check >> 4;

	Data left = compareCells(data >> 5, data);
	constexpr Data leftMask = repeat(0b00001, 1, 0);
	score += 3 * (left & ~randoms & leftMask).count();

	Data up = compareCells(data >> 5*5, data);
	constexpr Data upMask = repeat(0b00001, 0, 1);
	score += 3 * (up & ~randoms & upMask).count();

	Data leftRandom = (randoms >> 5 & randoms) | (randoms >> 5 & d123) | (d123 >> 5 & randoms);
	score += (leftRandom & leftMask).count();

	Data upRandom = (randoms >> 5*5 & randoms) | (randoms >> 5*5 & d123) | (d123 >> 5*5 & randoms);
	score += (upRandom & upMask).count();



	return score;
}