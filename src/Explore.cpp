
#include "Board.h"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <cassert>

#include "exploreNode.h"

constexpr size_t TRIM_SIZE = 256;
constexpr auto TIME_PER_ACTION = std::chrono::milliseconds(000);


uint64_t findBestMove(const Board& board) {
	
	// possibly better to save score locally instead of pointer to it
	std::vector<ExploreNode*> candidates;

	uint64_t bestScore = 0;
	auto sortBest = [&]() {
		std::partial_sort(
			candidates.begin(),
			candidates.begin() + std::min(TRIM_SIZE, candidates.size()),
			candidates.end(),
			[](const ExploreNode* a, const ExploreNode* b) { return a->score > b->score; }
		);
		if (candidates.size() > TRIM_SIZE)
			candidates.resize(TRIM_SIZE);

		ExploreNode* best = candidates.front();
		candidates.erase(candidates.begin());
		if (best->score > bestScore)
			bestScore = best->score;
		return best;
	};

	auto beginTime = std::chrono::steady_clock::now();

	std::vector<ExploreNode> topLevelMoves;
	board.iterateMoves([&](const Board& board, const Move& move) {
		topLevelMoves.push_back(ExploreNode(board, move));
	});
	// no more modifications to topLevelMoves after this point
	for (auto& node : topLevelMoves)
		candidates.push_back(&node);

	uint64_t cycles = candidates.size();

	while (std::chrono::steady_clock::now() - beginTime < TIME_PER_ACTION && candidates.size() > 0) {
		auto* best = sortBest();
		uint64_t startSize = candidates.size();
		best->explore(candidates);
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
}



//Candidate::Candidate(const Candidate& candidate) : node(candidate.node), score(candidate.score) { }

//Candidate::Candidate(const ExploreNode* node, const uint64_t& score) : node(node), score(score) { }