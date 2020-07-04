

#include "Explore.h"
#include "ExploreNode.h"

#include <chrono>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <queue>




unsigned long findBestMove(const Board& board) {
	
	// possibly better to save score locally instead of pointer to it
	Candidates candidates{};

	auto beginTime = std::chrono::steady_clock::now();

	std::vector<ExploreNode> topLevelMoves{};
	board.iterateMoves([&](const Board& nextBoard, const Move& move) {
		//std::cout << std::bitset<2>(move >> 30) << " " << std::bitset<5>(move >> 25) << " " << std::bitset<25>(move) << std::endl;
		topLevelMoves.push_back(ExploreNode(nextBoard, move));
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

	size_t bestIndex = 0;
	while (std::chrono::steady_clock::now() - beginTime < std::chrono::milliseconds{MS_PER_ACTION}  && candidates.size() > 0) {
		for (const auto& candidate : candidates)
			assert(candidate.node->info.board.data[127] == 0);
		
		const auto it = bestIndex != 0 ? candidates.begin() + bestIndex : std::max_element(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) { return a.score < b.score; });
		Candidate best = *it;
		candidates.erase(it);

		for (auto& candidate : candidates)
			assert(best.node != candidate.node);
		const auto& childNodes = best.node->explore();

		cycles += childNodes.size();
		bestIndex = 0;
		for (ExploreNode& node : childNodes) {
			assert(node.info.board.data[127] == 0);
			if (node.score > best.score) {
				best.score = node.score;
				bestIndex = candidates.size();
			}
			candidates.push_back(Candidate{ node.score, &node });
		}


		for (const auto& candidate : candidates)
			assert(candidate.node->info.board.data[127] == 0);
	}


	if (candidates.size() == 0)
		std::cout << "out of options, terminated search early" << std::endl;

	std::cout << "explored " << cycles << " boards. " << std::endl;

	if (topLevelMoves.size() <= 0)
		return 0;

	for (auto& node : topLevelMoves)
		node.computeScore();

	const auto& bestCandidate = std::max_element(topLevelMoves.begin(), topLevelMoves.end(), [](const ExploreNode& a, const ExploreNode& b) { return a.score < b.score; });

	for (auto& node : topLevelMoves) {
		uint64_t s = node.score >> 32;
		//std::cout << "score " << *(float*)&(s) << std::endl;
	}
	uint64_t s = bestCandidate->score >> 32;
	std::cout << "move rating: " << *(float*)&s << std::endl;
	std::cout << "candidates size: " << candidates.size() << std::endl;
	
	return bestCandidate->score & 0xffffffff;
}
