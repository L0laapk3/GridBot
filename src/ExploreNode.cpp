

#include "ExploreNode.h"

#include <cassert>



ExploreNode::ExploreNode(const Board& board, const unsigned long& move) {
	info.board = board;
	const float scoreFloat = board.eval();
	//std::cout << "hello! " << scoreFloat << std::endl;
	score = *(uint64_t*)&scoreFloat << 32 | move;
	assert(info.board.data[127] == 0 && info.board.data[126] == 0 && info.board.data[125] == 0);
}


void ExploreNode::explore(std::vector<Candidate>& candidates) {
	// will use the consumed wildcards in the lower 32 bits of prevScore
#ifndef NDEBUG
	assert(info.board.data[127] == 0 && info.board.data[126] == 0 && info.board.data[125] == 0);
#endif

	bool first = true;
	const uint64_t availableWildcards = shrink(andBits(info.board.data == repeat(0x0f)));
	std::vector<ExploreNode> childNodes;
	info.board.iterateMoves([&](const Board& board, const unsigned long& move) {
		childNodes.push_back(ExploreNode(board, move));
	});

	childNodes.shrink_to_fit();

	for (ExploreNode& node : childNodes) {
		assert(node.info.board.data[127] == 0);
		candidates.push_back(Candidate{ node.score, &node });
	}
	for (const auto& candidate : candidates)
		assert(candidate.node->info.board.data[127] == 0);

	info.childNodes = {
		childNodes.data(),
		availableWildcards << 32 | childNodes.size()
	};


	for (const auto& candidate : candidates)
		assert(candidate.node->info.board.data[127] == 0);

	info.board.data[127] = 1;


	for (const auto& candidate : candidates)
		assert(this != candidate.node);

	// shitty hack to steal ownership of the now array on the heap.
	new (&childNodes) std::vector<ExploreNode>; 

}


void ExploreNode::computeScore() {
	assert(info.board.data[126] == 0 && info.board.data[125] == 0);
	if (info.board.data[127] == 0) // this node has not been deepened, it has no child nodes so the score is directly the heuristic score
		return;
#ifndef NDEBUG
	info.board.data[126] = 1;
#endif

	// calculate score based on children, resolve dependencies in used wildcards and shit
	const auto& end = info.childNodes.begin + (info.childNodes.wildcards_size & 0xffffffff);
	for (auto* it = info.childNodes.begin; it < end; it++)
		(*it).computeScore();

	std::array<unsigned long, 3> wildcards{ info.childNodes.wildcards_size >> 32 };
	float partialScore = 0.f;
	float remainderChance = 1.f;
	for (auto it = info.childNodes.begin; it < end; it++) {
		std::partial_sort(it, it + 1, end, [](const ExploreNode& a, const ExploreNode& b) { return a.score > b.score; });
		const ExploreNode& node = *it;
		unsigned long consumedWildcards = (unsigned long)node.score;
		const unsigned short wildcardValue = (node.score >> 30) & 0x3;	// 00 = 2, 01 = 1, 10 = 3
		uint32_t scoreInt = node.score >> 32;
		node.~ExploreNode();
		if (consumedWildcards & ~wildcards[wildcardValue]) // move uses wildcard value that isn't available anymore (has 0% chance of occuring)
			continue;
		float& subScore = *(float*)(&scoreInt);
		std::cout << "subscore " << subScore << std::endl;
		assert(subScore < 1000);
		float chance = remainderChance;
		std::array<unsigned long, 2> wildcardCopies = { wildcards[wildcardValue == 0 ? 1 : 0], wildcards[wildcardValue == 2 ? 1 : 2] };
		for (int i = 0; i < 25; i++) {
			if (consumedWildcards & 0b1)
				chance /= (1 + (wildcardCopies[0] & 0b1) + (wildcardCopies[1] & 0b1));
			consumedWildcards >>= 1;
			wildcardCopies[0] >>= 1;
			wildcardCopies[1] >>= 1;
		}
		if (chance == 1.f) {
			partialScore += subScore;
			break;
		}

		partialScore += chance * subScore;
		remainderChance -= chance;

		wildcards[wildcardValue] &= ~consumedWildcards;
		for (const auto& wildcard : wildcards)	// remainderChance should be 0 by now but because of inaccuracies lets just check if any wildcards are left for use instead
			if (wildcard != 0)
				continue; // if any are left, continue

		break;
	}
	// if any of the wildcard combinations are done at this point, no moves have been found so the score is zero for those (death)
	// partialScore += remainderChance * 0;

	assert(partialScore < 1000);
	score = *(uint64_t*)&partialScore << 32 | (score & 0xffffffff);
	uint64_t s = score >> 32;
	//std::cout << "score " << partialScore << " " << *(float*)&(s) << std::endl;
}

void ExploreNode::cleanUp() {
	assert(info.board.data[126] == 0 && info.board.data[125] == 0);
	if (info.board.data[127] == 1)
		for (auto it = info.childNodes.begin; it < info.childNodes.begin + (info.childNodes.wildcards_size & 0xffffffff); it++)
			it->~ExploreNode();
#ifndef NDEBUG
	info.board.data[126] = 1;
#endif
}


ExploreNode::~ExploreNode() {
	// 127: vector was created
	// 126: vector was destructed again
	assert((info.board.data[127] == 0 && info.board.data[126] == 0 && info.board.data[125] == 0)
		|| (info.board.data[127] == 1 && info.board.data[126] == 1 && info.board.data[125] == 0));
}
