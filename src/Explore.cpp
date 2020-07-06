

#include "Explore.h"
#include "ExploreNode.h"

#include <chrono>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <queue>




float lastScore = -1;

std::vector<int> findBestMove(const Board& board) {


	auto beginTime = std::chrono::steady_clock::now();

	std::vector<ExploreNode> topLevelMoves{};
	board.iterateMoves(topLevelMoves);
	// no more modifications to topLevelMoves after this point (pointers matter)

	if (0) {
		std::sort(topLevelMoves.begin(), topLevelMoves.end(), std::greater<ExploreNode>());

		for (auto& node : topLevelMoves) {
			std::cout << "score " << node.getScore() << "\t";
			const auto& move = board.decompressMove(node.score);
			std::copy(move.begin(), move.end(), std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
		}
	}



	for (auto& node : topLevelMoves) {
		assert(candidates.back().node->info.board.data[127] == 0);
	}

	std::queue<std::span<ExploreNode>> queue;
	queue.push(std::span<ExploreNode>(topLevelMoves));

	std::cout << "depth 1: ratio " << topLevelMoves.size() << std::endl;
	size_t depth = 1;
	size_t lastSize = topLevelMoves.size();

	while (std::chrono::steady_clock::now() - beginTime < std::chrono::milliseconds{MS_PER_ACTION}) {
		size_t size = 0;
		size_t remaining = queue.size();
		while (remaining-- > 0) {
			for (ExploreNode& node : queue.front()) {
				const std::span<ExploreNode>& childNodes = node.explore();
				queue.push(childNodes);
				size += childNodes.size();
			}
			queue.pop();
		}

		std::cout << "depth " << ++depth << ": ratio " << (int)((float)size / lastSize) << " (size " << size << ")" << std::endl;
		lastSize = size;
		if (size == 0) {
			std::cout << "out of options, terminated search early" << std::endl;
			break;
		}
	}

	std::cout << "took " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - beginTime).count() << "ms." << std::endl;


	if (topLevelMoves.size() <= 0)
		return std::vector<int>();

	for (auto& node : topLevelMoves)
		node.computeScore();

	if (0) {
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
