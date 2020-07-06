

#include "Explore.h"
#include "ExploreNode.h"

#include <chrono>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <stack>



struct DepthNode {
	ExploreNode* node;
	ExploreNode* it;
};

std::vector<int> findBestMove(const Board& board, long timeBudget) {

	size_t depth = 0;
	size_t lastSize = 1;
	uint64_t bestScore;

	while (true) {
		const auto beginTime = std::chrono::steady_clock::now();

		size_t size = 0;
		bestScore = 0;

		std::stack<DepthNode> stack;
		std::vector<ExploreNode> topLevelMoves;
		board.iterateMoves(topLevelMoves);
		for (ExploreNode& node : topLevelMoves) {
			node.explore();
			stack.push(DepthNode{ &node, node.info.childNodes.begin + (node.info.childNodes.wildcards_size & 0xffffffff) });
			++size;

			while (!stack.empty()) {
				if (stack.top().it == stack.top().node->info.childNodes.begin || stack.size() > depth) {
					stack.pop();
					continue;
				}
				--stack.top().it;
				stack.top().it->explore();
				const size_t children = stack.top().it->info.childNodes.wildcards_size & 0xffffffff;
				stack.push(DepthNode{ stack.top().it, stack.top().it->info.childNodes.begin + children });
				size += children;
			}
			node.computeScore();

			if (node.score > bestScore)
				bestScore = node.score;
		}

		const float ratio = ((float)size / lastSize);
		const unsigned long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - beginTime).count();
		if (size == 0) {
			//std::cout << "game over" << std::endl;
			return std::vector<int>();
		}
		if (size == lastSize) {
			//std::cout << "out of options, terminated search early" << std::endl;
			break;
		}
		//std::cout << "depth " << ++depth << ": ratio " << (float)(int)(ratio*10)/10 << " in " << ms << "ms. (size " << size << ")" << std::endl;
		lastSize = size;
		timeBudget -= ms * ratio;
		if (timeBudget < 0)
			break;
	}


	return board.decompressMove(bestScore);
}
