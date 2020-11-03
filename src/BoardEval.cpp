#include "Board.h"

#include <algorithm>
#include <cmath>

#include <iostream>

constexpr float MERGES_WEIGHT = 100.f;
constexpr float MONOTONICITY_POWER = 4.f;
constexpr float MONOTONICITY_WEIGHT = -1.f;

float Board::eval() {
	float scoreMerges = 0;
	float scoreMonotonicity = 0;
	for (size_t m = 0; m <= 4; m += 4)
		for (size_t i = 0; i < 5; i++) {
			float monotonicityLeft = 0;
			float monotonicityRight = 0;
			for (size_t j = 0; j < 4; j++) {
				const auto aCell = data[(5 - m) * i + (1 + m) * j];
				const auto bCell = data[(5 - m) * i + (1 + m) * (j + 1)];
				// iterate random possibilities
				const float mult = (aCell ? 3 : 1) * (bCell ? 3 : 1);
				for (auto a = aCell; a < (aCell ? aCell + 1 : 3); a++)
					for (auto b = bCell; b < (bCell ? bCell + 1 : 3); b++) {

						if (a == b)
							scoreMerges += mult;

						if (a > b)
							monotonicityLeft += mult * (std::powf(a, MONOTONICITY_POWER) - std::powf(b, MONOTONICITY_POWER));
						else
							monotonicityRight += mult * (std::powf(b, MONOTONICITY_POWER) - std::powf(a, MONOTONICITY_POWER));
					}
			}
			scoreMonotonicity += std::min(monotonicityLeft, monotonicityRight);
		}

	scoreMerges *= MERGES_WEIGHT;
	scoreMonotonicity *= MONOTONICITY_WEIGHT;

	//std::cout << scoreMerges << " " << scoreMonotonicity << std::endl;
	return scoreMerges + scoreMonotonicity;
}