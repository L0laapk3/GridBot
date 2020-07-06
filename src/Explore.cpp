

#include "Explore.h"

#include <chrono>
#include <algorithm>
#include <iostream>
#include <cassert>









const float computeScore(const Board& board, const unsigned long& depth, size_t& count) {
	++count;
	if (depth == 0)
		return board.eval();


	struct Child {
		float score;
		Board board;
		unsigned long consumedWildcards;
		unsigned long wildcardValue;
	};

	const uint64_t wildcards = shrink(andBits(board.data == repeat(0x0f)));

	std::vector<Child> children;
	board.iterateMoves([&](const Board& board, const unsigned long& move, const unsigned long& wildcardValue, const unsigned long& endPos) {
		children.push_back({
			computeScore(board, depth - 1, count),
			board,
			move & wildcards,
			wildcardValue
			});
		});

	std::array<unsigned long, 3> remainingWildcards = { wildcards, wildcards, wildcards };

	float partialScore = 0.f;
	float remainderChance = 1.f;

	for (auto it = children.begin(); it < children.end(); it++) {
		std::partial_sort(it, it + 1, children.end(), [](const Child& a, const Child& b) { return a.score > b.score; });

		float chance = remainderChance;
		if (it->consumedWildcards) {
			if (it->consumedWildcards & ~remainingWildcards[it->wildcardValue]) // check if a wildcard is used that is no longer available.
				continue;
			std::array<unsigned long, 2> wildcardCopies = { remainingWildcards[it->wildcardValue == 0 ? 1 : 0], remainingWildcards[it->wildcardValue == 2 ? 1 : 2] };
			for (int i = 0; i < 25; i++) {
				if (it->consumedWildcards & 0b1) {
					chance /= (1.f + (wildcardCopies[0] & 0b1) + (wildcardCopies[1] & 0b1));
				}
				it->consumedWildcards >>= 1;
				wildcardCopies[0] >>= 1;
				wildcardCopies[1] >>= 1;
			}
			//std::cout << "subscore " << subScore << " chance " << chance << " tot " << partialScore << std::endl;
		}

		partialScore += chance * it->score;

		//std::cout << "tot " << partialScore << std::endl;
		if (chance == remainderChance)
			break;
		remainderChance -= chance;

		remainingWildcards[it->wildcardValue] &= ~it->consumedWildcards;

		for (const auto& wildcard : remainingWildcards)	// remainderChance should be 0 by now but because of inaccuracies lets just check if any wildcards are left for use instead
			if (wildcard != 0)
				goto cnt; // if any are left, continue
		break;
	cnt:;
	}

	return partialScore;
}



std::vector<unsigned long> findBestMove(const Board& board, long timeBudget) {

	struct Action {
		float score;
		unsigned long move;
		unsigned long endPos;
	};


	size_t depth = 0;
	size_t lastCount = 1;
	Action bestAction{ 0, 0 };

	while (true) {
		const auto beginTime = std::chrono::steady_clock::now();

		size_t count = 0;
		bestAction = { 0, 0 };

		std::vector<Action> Actions;
		board.iterateMoves([&](const Board& board, const unsigned long& move, const unsigned long& wildcardValue, const unsigned long& endPos) {
			++count;
			const float score = computeScore(board, depth, count);
			Actions.push_back({ score, move, endPos });
			if (score > bestAction.score)
				bestAction = { score, move, endPos };
		});


		const float ratio = ((float)count / lastCount);
		const unsigned long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - beginTime).count();
		if (count == 0) {
			std::cout << "game over" << std::endl;
			return std::vector<unsigned long>();
		}
		if (count == lastCount && depth > 1) {
			std::cout << "out of options, terminated search early" << std::endl;
			break;
		}
		std::cout << "depth " << ++depth << ": ratio " << (float)(int)(ratio*10)/10 << " in " << ms << "ms. (size " << count << ")" << std::endl;
		lastCount = count;
		timeBudget -= ms * ratio;
		if (timeBudget < 0)
			break;
	}


	return board.decompressMove(bestAction.move, bestAction.endPos);
}