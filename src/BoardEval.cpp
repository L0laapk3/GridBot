
#include "Board.h"

#include <iostream>


// basic eval that counts how many connections each cell has
const float Board::eval() const {
	std::cout << std::endl;
	print();

	float scoreSurvivability = 0;

	constexpr Data randomMask = repeat(0x0f);
	Data randoms = andBits(data == randomMask);
	std::cout << randoms << std::endl;

	//Data d123 = (((data ^ data >> 1) & data >> 4) | ((data & data >> 1) & ~(data >> 4))) & ~(data >> 2 | data >> 3);
	Data d123 = andBits(data == repeat(0x11)) | andBits(data == repeat(0x01)) | andBits(data == repeat(0x02));

	Data left = andBits(data == data >> 5);
	constexpr Data leftMask = repeat(0x01, 1, 0);
	scoreSurvivability += 3 * (left & ~randoms & leftMask).count();

	Data up = andBits(data == data >> 5*5);
	constexpr Data upMask = repeat(0x01, 0, 1);
	scoreSurvivability += 3 * (up & ~randoms & upMask).count();

	//printRaw(randoms);
	//std::cout << "^ randoms, \\/ d123" << std::endl;
	//printRaw(d123);

	Data leftRandom = (randoms >> 5 & randoms) | (randoms >> 5 & d123) | (d123 >> 5 & randoms);
	scoreSurvivability += (leftRandom & leftMask).count();

	Data upRandom = (randoms >> 5*5 & randoms) | (randoms >> 5*5 & d123) | (d123 >> 5*5 & randoms);
	scoreSurvivability += (upRandom & upMask).count();

	std::cout << "calculated score: " << scoreSurvivability << std::endl;

	return scoreSurvivability;
}