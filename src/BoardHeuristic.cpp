
#include "Board.h"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <cassert>

constexpr size_t TRIM_SIZE = 256;
constexpr auto TIME_PER_MOVE = std::chrono::milliseconds(000);

struct Candidate {
	uint64_t score;
	Board board;
};


uint32_t Board::findBestMove() const {

	std::vector<Candidate> candidates{};

	uint64_t bestScore = 0;
	auto sortBest = [&]() {
		std::partial_sort(
			candidates.begin(),
			candidates.begin() + std::min(TRIM_SIZE, candidates.size()),
			candidates.end(),
			[](const Candidate& a, const Candidate& b) { return a.score > b.score; }
		);
		if (candidates.size() > TRIM_SIZE)
			candidates.resize(TRIM_SIZE);

		const Candidate best = candidates.front();
		candidates.erase(candidates.begin());
		if (best.score > bestScore)
			bestScore = best.score;
		return best;
	};

	uint64_t cycles = 0;
	auto addCandidate = [&](const Candidate& candidate) {
		//std::cout << "added " << std::bitset<30>(candidate.score) << std::endl;
		candidates.push_back(candidate);
		cycles++;
	};

	auto beginTime = std::chrono::steady_clock::now();

	iterateMoves([&](const Board& board, const Move& move) {
		assert((move & 0x1ffffff) != 0b0);
		//std::cout << std::bitset<25>(move) << std::endl;
		addCandidate(Candidate{ ((uint64_t)board.eval() << 32) + move, board });
	});

	while (std::chrono::steady_clock::now() - beginTime < TIME_PER_MOVE && candidates.size() > 0) {
		auto best = sortBest();
		best.board.iterateMoves([&](const Board& board, const Move& move) {
			addCandidate(Candidate{ ((uint64_t)board.eval() << 32) + (best.score & 0xffffffff), board });
		});
	}

	if (candidates.size() > 0)
		sortBest();
	else
		std::cout << "out of options, terminated search early" << std::endl;

	std::cout << "explored " << cycles << " boards. " << std::bitset<30>(bestScore & 0x1ffffff) << std::endl;
	//for (const auto candidate : candidates) 
	//	std::cout << (candidate.score >> 32) << " " << std::bitset<25>(candidate.score & 0x1ffffff) << std::endl;

	return bestScore & 0xffffffff;
}