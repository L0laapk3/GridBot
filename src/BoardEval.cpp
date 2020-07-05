
#include "Board.h"

#include <iostream>



constexpr std::array<float, 13> WEIGHTS{ 0, 0, 0, 0,0,0,0,0, 0, 0, 1, 4, 16 };
constexpr std::array<float, 25> ALTMAP1{ 4, 3, 2, 1, 0, 9, 8, 7, 6, 5, 14, 13, 12, 11, 10, 19, 18, 17, 16, 15, 24, 23, 22, 21, 20 };
constexpr std::array<float, 25> ALTMAP2{ 0, 5, 10, 15, 20, 1, 6, 11, 16, 21, 2, 7, 12, 17, 22, 3, 8, 13, 18, 23, 4, 9, 14, 19, 24 };
constexpr std::array<float, 25> ALTMAP3{ 20, 15, 10, 5, 0, 21, 16, 11, 6, 1, 22, 17, 12, 7, 2, 23, 18, 13, 8, 3, 24, 19, 14, 9, 4 };


// basic eval that counts how many connections each cell has
const float Board::eval() const {

	float scoreSurvivability = 0;

	//possible error?
	const Data randoms = andBits(data == repeat(0x0f)) & repeat(0x1);
	const Data randomMask = toMask(randoms);
	const Data noRandoms = data & ~randomMask;

	//Data d123 = (((data ^ data >> 1) & data >> 4) | ((data & data >> 1) & ~(data >> 4))) & ~(data >> 2 | data >> 3);
	const Data d123 = andBits(data == repeat(0x11)) | andBits(data == repeat(0x01)) | andBits(data == repeat(0x02));

	{
		const Data left = andBits(data == data >> 5);
		constexpr Data leftMask = repeat(0x01, 1, 0);
		scoreSurvivability += 3 * (left & ~randomMask & leftMask).count();

		const Data up = andBits(data == data >> 5 * 5);
		constexpr Data upMask = repeat(0x01, 0, 1);
		scoreSurvivability += 3 * (up & ~randomMask & upMask).count();

		//printRaw(randoms);
		//std::cout << "^ randoms, \\/ d123" << std::endl;
		//printRaw(d123);

		const Data leftRandom = (randoms >> 5 & randoms) | (randoms >> 5 & d123) | (d123 >> 5 & randoms);
		scoreSurvivability += (leftRandom & leftMask).count();

		const Data upRandom = (randoms >> 5 * 5 & randoms) | (randoms >> 5 * 5 & d123) | (d123 >> 5 * 5 & randoms);
		scoreSurvivability += (upRandom & upMask).count();
	}
	//std::cout << "calculated score: " << scoreSurvivability << std::endl;



	float scoreMonotonicity = 0;
	const Data withoutThrees = ((data ^ randomMask) & repeat(0x0f)) + ((data ^ randomMask) >> 4 & repeat(0x01));
	{
		const Data leftSubtract = withoutThrees + (withoutThrees >> 5 ^ repeat(0x0f)) + repeat(0x01);
		const Data leftRowHalfSum = (leftSubtract & repeatRow(0b111110000011111)) + ((leftSubtract & repeatRow(0b11111000001111100000)) >> 5);
		const Data leftRowSum = (leftRowHalfSum & repeatRow(0b111111)) + ((leftRowHalfSum & repeatRow(0b1111110000000000)) >> 10);
		const Data leftRowNeg = ~leftRowSum & repeatRow(0b1000000);
		const Data leftRowAbs = (leftRowSum ^ (leftRowNeg >> 1 | leftRowNeg >> 2 | leftRowNeg >> 3 | leftRowNeg >> 4 | leftRowNeg >> 5 | leftRowNeg >> 6)) + (leftRowNeg >> 6);
		scoreMonotonicity += (leftRowAbs & Data(0x3f)).to_ullong()
			+ (leftRowAbs >>  5*5 & Data(0x3f)).to_ullong()
			+ (leftRowAbs >> 10*5 & Data(0x3f)).to_ullong()
			+ (leftRowAbs >> 15*5 & Data(0x3f)).to_ullong()
			+ (leftRowAbs >> 20*5 & Data(0x3f)).to_ullong();

		//const Data leftNegative = leftSubtract & repeat(0x10, 1, 0);
		//const Data leftAbs = (leftSubtract & repeat(0x0f, 1, 0)) ^ (leftNegative >> 1 | leftNegative >> 2 | leftNegative >> 3 | leftNegative >> 4);
		//scoreContinuity += leftNegative.count();
		


		const Data upSubtract = withoutThrees + (withoutThrees >> 25 ^ repeat(0x0f)) + repeat(0x01);
		constexpr Data firstHalf = repeatRow(0b111110000011111);// raw(0x1f, 0, 0x1f, 0, 0x1f, 0x1f, 0, 0x1f, 0, 0x1f, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0);
		constexpr Data secondHalf = repeatRow(0b11111000001111100000);// raw(0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f);
		const Data upColHalfSumFirstHalf = (firstHalf & repeatCol(0x1f, 0, 0x1f) & upSubtract) + ((firstHalf & repeatCol(0, 0x1f, 0, 0x1f) & upSubtract) >> 25);
		const Data upColHalfSumSecondHalf = (secondHalf & repeatCol(0x1f, 0, 0x1f) & upSubtract) + ((secondHalf & repeatCol(0, 0x1f, 0, 0x1f) & upSubtract) >> 25);
		const uint64_t upColSumFirstHalf = (upColHalfSumFirstHalf & repeatCol(0x3f)).to_ullong() + ((upColHalfSumFirstHalf & repeatCol(0, 0, 0x3f)) >> 50).to_ullong();
		const uint64_t upColSumSecondHalf = (upColHalfSumSecondHalf & repeatCol(0x3f)).to_ullong() + ((upColHalfSumSecondHalf & repeatCol(0, 0, 0x3f)) >> 50).to_ullong();
		const uint64_t upColNegFirstHalf = ~upColSumFirstHalf & ((firstHalf << 2) & repeatCol(0b1000000)).to_ullong();
		const uint64_t upColNegSecondHalf = ~upColSumSecondHalf & ((secondHalf << 2) & repeatCol(0b1000000)).to_ullong();
		const uint64_t upColAbsFirstHalf = (upColSumFirstHalf ^ (upColNegFirstHalf >> 1 | upColNegFirstHalf >> 2 | upColNegFirstHalf >> 3 | upColNegFirstHalf >> 4 | upColNegFirstHalf >> 5 | upColNegFirstHalf >> 6)) + (upColNegFirstHalf >> 6);
		const uint64_t upColAbsSecondHalf = (upColSumSecondHalf ^ (upColNegSecondHalf >> 1 | upColNegSecondHalf >> 2 | upColNegSecondHalf >> 3 | upColNegSecondHalf >> 4 | upColNegSecondHalf >> 5 | upColNegSecondHalf >> 6)) + (upColNegSecondHalf >> 6);
		scoreMonotonicity += (upColAbsFirstHalf & 0x3f)
			+ (upColAbsSecondHalf >> 5 & 0x3f)
			+ (upColAbsFirstHalf >> 10 & 0x3f)
			+ (upColAbsSecondHalf >> 15 & 0x3f)
			+ (upColAbsFirstHalf >> 20 & 0x3f);

		//printRaw(upColSum);


		//std::cout << scoreContinuity << std::endl;

		//const Data upNegative = upSubtract & repeat(0x10, 0, 1);
		//const Data upAbs = (upSubtract & repeat(0x0f, 0, 1)) ^ (upNegative >> 1 | upNegative >> 2 | upNegative >> 3 | upNegative >> 4);
		//scoreContinuity += upNegative.count();
	}



	std::array<float, 4> positionalScore{ 0, 0, 0, 0 };
	Data d = data;
	for (int i = 0; i < 25; i++) {
		const float value = (d & Data(0x1f) ^ Data(0x0f)).any() ? (d[4] ? 3 : 1) * (1 << (d & Data(0x0f)).to_ulong()) : 2;
		d >>= 5;
		positionalScore[0] += WEIGHTS[std::abs(i - 12)] * value;
		positionalScore[1] += WEIGHTS[std::abs(ALTMAP1[i] - 12)] * value;
		positionalScore[1] += WEIGHTS[std::abs(ALTMAP2[i] - 12)] * value;
		positionalScore[1] += WEIGHTS[std::abs(ALTMAP3[i] - 12)] * value;
	}

	const float scoreWeighted = *std::max_element(positionalScore.begin(), positionalScore.end());






	const Data threes = noRandoms & repeat(0x10);
	const Data powerOfTwo = (noRandoms & repeat(0xf) & (noRandoms >> 1 | noRandoms >> 2 | noRandoms >> 3 | repeat(0xe))) | randoms;
	const Data hasThree = ~powerOfTwo | threes >> 1 | threes >> 2 | threes >> 3 | threes >> 4;

	//std::cout << (hasThree & repeat(0x1)).count() << " " << (hasThree & repeat(0x2)).count() << " " << (hasThree & repeat(0x4)).count() << " " << (hasThree & repeat(0x8)).count() << std::endl;

	//print();
	//printRaw(hasThree);

	const float scoreThrees = (hasThree & repeat(0x1)).count()
		+ (hasThree & repeat(0x2)).count() * 2*2
		+ (hasThree & repeat(0x4)).count() * 4*4
		+ (hasThree & repeat(0x8)).count() * 8*8;



	return 6 * scoreSurvivability + scoreThrees;// +scoreWeighted / 8;// +4 * scoreContinuity;
}