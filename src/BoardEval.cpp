
#include "Board.h"

#include <iostream>


// basic eval that counts how many connections each cell has
const float Board::eval() const {
	float scoreSurvivability = 0;

	constexpr Data randomMask = repeat(0b01111);
	Data randoms = andBits(data == randomMask);

	Data d123 = andBits(~(((data | data >> 1)& data >> 4) | (data & data >> 1) | data >> 2 | data >> 3));

	Data left = andBits(data == data >> 5);
	constexpr Data leftMask = repeat(0b00001, 1, 0);
	scoreSurvivability += 3 * (left & ~randoms & leftMask).count();

	Data up = andBits(data == data >> 5*5);
	constexpr Data upMask = repeat(0b00001, 0, 1);
	scoreSurvivability += 3 * (up & ~randoms & upMask).count();

	Data leftRandom = (randoms >> 5 & randoms) | (randoms >> 5 & d123) | (d123 >> 5 & randoms);
	scoreSurvivability += (leftRandom & leftMask).count();

	Data upRandom = (randoms >> 5*5 & randoms) | (randoms >> 5*5 & d123) | (d123 >> 5*5 & randoms);
	scoreSurvivability += (upRandom & upMask).count();

	//std::cout << "calculated score: " << score << std::endl;

	return scoreSurvivability;
}