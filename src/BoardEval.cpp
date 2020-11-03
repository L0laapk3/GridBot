#include "Board.h"

#include <algorithm>
#include <cmath>

#include <iostream>

constexpr float SCORES_MERGES_WEIGHT = 100.f;
constexpr float SCORES_MONOTONICITY_POWER = 4.f;
constexpr float SCORES_MONOTONICITY_WEIGHT = -1.f;
constexpr float SCORES_VALUES_WEIGHT = -20000.f;
constexpr float SCORES_MULTIPLE_9_PENALTY = 100.f;
constexpr float SCORES_MULTIPLE_3_POWER = 1.f;

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

						if (a > b)
							monotonicityLeft += mult * (std::powf(a, SCORES_MONOTONICITY_POWER) - std::powf(b, SCORES_MONOTONICITY_POWER));
						else
							monotonicityRight += mult * (std::powf(b, SCORES_MONOTONICITY_POWER) - std::powf(a, SCORES_MONOTONICITY_POWER));
					}
			}
			scoreMonotonicity += std::min(monotonicityLeft, monotonicityRight);
		}


	for (size_t i = 0; i < 25; i++) {
		const float mult = data[i] ? 1 : 3;
		for (auto value = data[i]; value < (data[i] ? data[i] + 1 : 3); value++) {
			if (data[i] % 9 == 0)
				scoreValues += mult * SCORES_MULTIPLE_9_PENALTY;
			else if (data[i] % 3 != 0) {
				scoreValues += mult * std::powf(data[i], SCORES_MULTIPLE_3_POWER);
			}
		}
	}

	scoreMerges *= SCORES_MERGES_WEIGHT;
	scoreMonotonicity *= SCORES_MONOTONICITY_WEIGHT;
	scoreValues *= SCORES_VALUES_WEIGHT;

	//std::cout << scoreMerges << " " << scoreMonotonicity << " " << scoreValues << std::endl;
	return scoreMerges + scoreMonotonicity + scoreValues;
}