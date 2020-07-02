#pragma once

#include <vector>

#include "board.h"


class ExploreNode {
	public:
		ExploreNode(const ExploreNode& exploreNode) = default;
		ExploreNode(const Board& board, const uint64_t& score);

		void Explore();
		void computeScore();

		~ExploreNode();

	private:
		uint64_t score; // upper 32 bits score, lower 32 bits consumed wildcards
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
