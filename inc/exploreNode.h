#pragma once

#include <vector>

#include "Board.h"


class ExploreNode {
	public:
		ExploreNode(const ExploreNode& exploreNode) = default;
		ExploreNode(const Board& board, const unsigned long& move);

		void explore(std::vector<ExploreNode*>& candidates);
		void computeScore();

		~ExploreNode();

		uint64_t score; // upper 32 bits score, lower: 2b used wildcard value | 5b end cell location | 25b bitboard with moved cells
		union Info {
			Board board;
			struct childNodes {
				ExploreNode* begin;
				uint64_t wildcards_size; // upper 32 bits available wildcards, lower 32 bits size of children
			} childNodes;
			~Info() {};
			Info(const Info& info) { board = info.board; };
			Info() {};
		} info;
};
