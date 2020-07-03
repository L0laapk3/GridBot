

#include "Explore.h"

#include <chrono>
#include <algorithm>
#include <iostream>
#include <cassert>


constexpr size_t TRIM_SIZE = 1024;
constexpr auto TIME_PER_ACTION = std::chrono::milliseconds(1000);


unsigned long findBestMove(const Board& board) {
	
	// possibly better to save score locally instead of pointer to it
	std::vector<Candidate> candidates{};

	auto sortBest = [&]() {
		for (const auto& candidate : candidates)
			assert(candidate.node->info.board.data[127] == 0);

		for (int i = 0; i < candidates.size(); i++)
			for (int j = i + 1; j < candidates.size(); j++)
				assert(candidates[i].node != candidates[j].node);

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

		for (auto& candidate : candidates)
			assert(&best != &candidate);

		for (auto& candidate : candidates)
			assert(best.node != candidate.node);

		return best.node;
	};

	auto beginTime = std::chrono::steady_clock::now();

	std::vector<ExploreNode> topLevelMoves{};
	board.iterateMoves([&](const Board& board, const Move& move) {
		//std::cout << std::bitset<2>(move >> 30) << " " << std::bitset<5>(move >> 25) << " " << std::bitset<25>(move) << std::endl;
		topLevelMoves.push_back(ExploreNode(board, move));
		assert(topLevelMoves.back().info.board.data[127] == 0);
	});
	// no more modifications to topLevelMoves after this point


	for (auto& node : topLevelMoves) {
		candidates.push_back(Candidate{ node.score, &node });
		assert(candidates.back().node->info.board.data[127] == 0);
	}
	for (const auto& candidate : candidates)
		assert(candidate.node->info.board.data[127] == 0);

	uint64_t cycles = candidates.size();

	while (std::chrono::steady_clock::now() - beginTime < TIME_PER_ACTION && candidates.size() > 0) {
		for (const auto& candidate : candidates)
			assert(candidate.node->info.board.data[127] == 0);
		auto* best = sortBest();
		for (auto& candidate : candidates)
			assert(best != candidate.node);
		uint64_t startSize = candidates.size();
		best->explore(candidates);

		for (const auto& candidate : candidates)
			assert(candidate.node->info.board.data[127] == 0);
		cycles += candidates.size() - startSize;
	}

	if (candidates.size() > 0)
		sortBest();
	else
		std::cout << "out of options, terminated search early" << std::endl;

	std::cout << "explored " << cycles << " boards. " << std::endl;

	if (topLevelMoves.size() <= 0)
		return 0;

	for (auto& node : topLevelMoves)
		node.computeScore();

	const auto& bestCandidate = std::max_element(topLevelMoves.begin(), topLevelMoves.end(), [](const ExploreNode& a, const ExploreNode& b) { return a.score < b.score; });

	//for (auto& node : topLevelMoves) {
	//	uint64_t s = node.score >> 32;
	//	std::cout << "score " << *(float*)&(s) << std::endl;
	//}
	uint64_t s = bestCandidate->score >> 32;
	std::cout << "bestScore " << *(float*)&(s) << std::endl;

	return bestCandidate->score & 0xffffffff;
}
