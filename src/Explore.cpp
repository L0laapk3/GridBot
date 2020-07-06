

#include "Explore.h"
#include "ExploreNode.h"

#include <chrono>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <queue>




float lastScore = -1;

std::vector<int> findBestMove(const Board& board) {
	
	// possibly better to save score locally instead of pointer to it
	Candidates candidates{};

	auto beginTime = std::chrono::steady_clock::now();

	std::vector<ExploreNode> topLevelMoves{};
	board.iterateMoves(topLevelMoves);
	// no more modifications to topLevelMoves after this point (pointers matter)

	if (1) {
		std::sort(topLevelMoves.begin(), topLevelMoves.end(), std::greater<ExploreNode>());

		for (auto& node : topLevelMoves) {
			std::cout << "score " << node.getScore() << "\t";
			const auto& move = board.decompressMove(node.score);
			std::copy(move.begin(), move.end(), std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
		}
	}



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
		
		const auto it = bestIndex != 0 ? candidates.begin() + bestIndex : std::max_element(candidates.begin(), candidates.end());
		Candidate best = *it;
		candidates.erase(it);

		for (auto& candidate : candidates)
			assert(best.node != candidate.node);
		const auto& childNodes = best.node->explore();

		cycles += childNodes.size();
		bestIndex = 0;
		for (ExploreNode& node : childNodes) {
			assert(node.info.board.data[127] == 0);
			if (node > best) {
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
		return std::vector<int>();

	for (auto& node : topLevelMoves)
		node.computeScore();

	if (1) {
		std::sort(topLevelMoves.begin(), topLevelMoves.end(), std::greater<ExploreNode>());

		for (auto& node : topLevelMoves) {
			std::cout << node.info.board.data[127] << " score " << node.getScore() << "\t";
			const auto& move = board.decompressMove(node.score);
			std::copy(move.begin(), move.end(), std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
		}
	}

	const auto& bestCandidate = std::max_element(topLevelMoves.begin(), topLevelMoves.end());


	assert(lastScore != 0 || bestCandidate->getScore() == 0);
	lastScore = bestCandidate->getScore();

	std::cout << "move rating: " << bestCandidate->getScore() << std::endl;
	//std::cout << "candidates size: " << candidates.size() << std::endl;
	
	return board.decompressMove(bestCandidate->score);
}
