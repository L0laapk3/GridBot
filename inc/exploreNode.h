#pragma once

#include <vector>

#include "board.h"

class ExploreNode {
	public:
		ExploreNode(const ExploreNode& exploreNode);
		ExploreNode(const Board& board, const uint64_t& score);

		std::vector<ExploreNode>& Explore(const uint64_t& prevScore);
		void computeScore();

		~ExploreNode();

	private:
		union data {
			Board board;
			struct {
				uint64_t wildcardInfo;	// wildcard locations
				std::vector<ExploreNode> childNodes;
			} deep;
			~data() {};
		} info;
		uint64_t score; // upper 32 bits score, lower 32 bits consumed wildcards
};