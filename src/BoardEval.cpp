#include "Board.h"

#include <algorithm>
#include <cmath>
#include <bitset>
#include <limits>

#include <iostream>

constexpr float SCORES_MERGES_WEIGHT = 100.f;
constexpr float SCORES_MONOTONICITY_POWER = 3.5f;
constexpr float SCORES_MONOTONICITY_WEIGHT = 1.5f;
constexpr float SCORES_VALUES_WEIGHT = 800000.f;
constexpr float SCORES_NOT_MULTIPLE_2_PENALTY = std::numeric_limits<float>::max();
constexpr float SCORES_MULTIPLE_3_POWER = 1.5f;

float Board::eval() const {
	float scoreMerges = 0;
	float scoreMonotonicity = 0;
	float scoreValues = 0;
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
						else {
							const auto aPow2 = a % 3 ? a / 3 : a;
							const auto bPow2 = b % 3 ? b / 3 : b;
							if (aPow2 > bPow2)
								monotonicityLeft += mult * (std::powf(aPow2, SCORES_MONOTONICITY_POWER) - std::powf(bPow2, SCORES_MONOTONICITY_POWER));
							else
								monotonicityRight += mult * (std::powf(bPow2, SCORES_MONOTONICITY_POWER) - std::powf(aPow2, SCORES_MONOTONICITY_POWER));
						}
					}
			}
			scoreMonotonicity += std::min(monotonicityLeft, monotonicityRight);
		}


	for (size_t i = 0; i < 25; i++) {
		const float mult = data[i] ? 1 : 3;
		for (auto value = data[i]; value < (data[i] ? data[i] + 1 : 3); value++) {

			const auto n = data[i] % 3 ? data[i] : data[i] / 3;
			if ((n & (n - 1)) != 0) // is not 2^n or 3*2^n
				scoreValues += mult * SCORES_NOT_MULTIPLE_2_PENALTY;
			else if (data[i] % 3 != 0)
				scoreValues += mult * std::powf(data[i], SCORES_MULTIPLE_3_POWER);
		}
	}

	scoreMerges *= SCORES_MERGES_WEIGHT;
	scoreMonotonicity *= -SCORES_MONOTONICITY_WEIGHT;
	scoreValues *= -SCORES_VALUES_WEIGHT;

	//std::cout << scoreMerges << " " << scoreMonotonicity << " " << scoreValues << std::endl;
	return scoreMerges + scoreMonotonicity + scoreValues;
}