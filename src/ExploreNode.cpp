
#include "exploreNode.h"
#include <cassert>

ExploreNode::ExploreNode(const Data& data) : data(data) {
	assert(data[127] == 0 && data[126] == 0 && data[125] == 0);
}

std::vector<ExploreNode>* ExploreNode::Explore() {
	#ifndef NDEBUG
		assert(data[127] == 0 && data[126] == 0 && data[125] == 0);
		data[127] = 1;
	#endif
	
}

uint64_t ExploreNode::getScore() {
	return uint64_t();
}

ExploreNode::~ExploreNode() {
}
