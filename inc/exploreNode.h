#pragma once

#include <span>

#include "Board.h"
#include "Explore.h"


class ExploreNode {
	public:
		ExploreNode(const ExploreNode& exploreNode) = default;
		ExploreNode(const Board& board, const unsigned long& move);
		~ExploreNode();

		// once explore is called, the union will switch, it will have a bunch of child nodes, afterwards computeScore OR cleanup have to be called.
		std::span<ExploreNode> explore();

		void computeScore();
		void cleanUp();

		uint64_t score; // upper 32 bits score, lower: 2b used wildcard value | 5b end cell location | 25b bitboard with moved cells
		union Info {
			Board board;
			struct childNodes {
				ExploreNode* begin; // array
				uint64_t wildcards_size; // upper 32 bits: 25b wildcard locations on board, lower 32 bits size of children array
			} childNodes;
			Info(const Info& info) { board = info.board; };
			Info() {};
			~Info() {};
		} info;

	private:
		void dealloc();
};
