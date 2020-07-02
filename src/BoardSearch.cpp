
#include "Board.h"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <cassert>

constexpr size_t TRIM_SIZE = 256;
constexpr auto TIME_PER_MOVE = std::chrono::milliseconds(1000);


uint32_t Board::findBestMove() const {
	/*
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

	auto beginTime = std::chrono::steady_clock::now();

	iterateMoves([&](const Board& board, const Move& move) {
		assert((move & 0x1ffffff) != 0b0);
		//std::cout << std::bitset<25>(move) << std::endl;
		candidates.push_back(Candidate(board, move));
	});

	uint64_t cycles = candidates.size();

	while (std::chrono::steady_clock::now() - beginTime < TIME_PER_MOVE && candidates.size() > 0) {
		auto best = sortBest();
		uint64_t startSize = candidates.size();
		best.board.iterateMoves([&](const Board& board, const Move& move) {
			candidates.push_back(Candidate(board, move));
		});
		cycles += candidates.size() - startSize;
	}

	if (candidates.size() > 0)
		sortBest();
	else
		std::cout << "out of options, terminated search early" << std::endl;

	std::cout << "explored " << cycles << " boards. " << std::bitset<30>(bestScore & 0x1ffffff) << std::endl;
	//for (const auto candidate : candidates) 
	//	std::cout << (candidate.score >> 32) << " " << std::bitset<25>(candidate.score & 0x1ffffff) << std::endl;

	return bestScore & 0xffffffff;
	*/
	return 0;
}



//Candidate::Candidate(const Candidate& candidate) : node(candidate.node), score(candidate.score) { }

//Candidate::Candidate(const ExploreNode* node, const uint64_t& score) : node(node), score(score) { }