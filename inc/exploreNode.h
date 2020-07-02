#pragma once

#include "board.h"
#include <vector>


class ExploreNode;
struct ExploredNodeInfo {
	uint64_t wildcardInfo;
	std::vector<ExploreNode>* childNodes;
};
struct CollapsedNodeInfo {
	uint64_t wildcardInfo;
	uint64_t score;
};

class ExploreNode {
	public:
		ExploreNode(const Data& data);

		std::vector<ExploreNode>* Explore();
		uint64_t getScore();

		~ExploreNode();

	private:
		Data data;
};