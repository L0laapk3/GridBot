

#include "ExploreNode.h"
#include <cassert>

ExploreNode::ExploreNode(const Board& board, const uint64_t& score) : score(score) {
	info.board = board;
	assert(info.board.data[127] == 0 && info.board.data[126] == 0 && info.board.data[125] == 0);
}


void ExploreNode::Explore() {
	// will use the consumed wildcards in the lower 32 bits of prevScore
#ifndef NDEBUG
	assert(info.board.data[127] == 0 && info.board.data[126] == 0 && info.board.data[125] == 0);
#endif

	bool first = true;
	const uint64_t availableWildcards = shrink(andBits(info.board.data == repeat(0x0f)));
	std::vector<ExploreNode> childNodes;
	MoveFunc foundMove = [&](const Board& board, const unsigned long& consumedWildcards) {
		const float& score = board.eval();
		childNodes.push_back(ExploreNode(board, ((uint64_t)reinterpret_cast<const uint32_t&>(score) << 32) | consumedWildcards));
	};
	info.board.iterateMoves(foundMove);

	childNodes.shrink_to_fit();

	info.childNodes = {
		childNodes.data(),
		availableWildcards << 32 | childNodes.size()
	};

	// shitty hack to steal ownership of the now array on the heap.
	new (&childNodes) std::vector<ExploreNode>; 
}


void ExploreNode::computeScore() {
#ifndef NDEBUG
	assert((info.board.data[127] == 0 && info.board.data[126] == 0 && info.board.data[125] == 0)
		|| (info.board.data[127] == 1 && info.board.data[126] == 0 && info.board.data[125] == 0));
	info.board.data[126] = 1;
#endif
	if (info.board.data[127] == 0) // this node has not been deepened, it has no child nodes so the score is directly the heuristic score
		return;

	// calculate score based on children, resolve dependencies in used wildcards and shit
	const auto& end = info.childNodes.begin + (info.childNodes.wildcards_size & 0xffffffff);
	for (auto i = info.childNodes.begin; i < end; i++)
		(*i).computeScore();

	std::array<unsigned long, 3> wildcards{ info.childNodes.wildcards_size >> 32 };
	float partialScore = 0;
	float remainderChance = 1;
	for (auto i = info.childNodes.begin; i < end; i++) {
		std::partial_sort(i, i + 1, end, [](const ExploreNode& a, const ExploreNode& b) { return a.score > b.score; });
		const ExploreNode& node = *i;
		unsigned long consumedWildcards = (unsigned long)node.score;
		const unsigned short wildcardValue = (node.score >> 25) & 0x3;
		if (consumedWildcards & ~wildcards[wildcardValue])
			continue;	// move uses wildcard value that isn't available anymore (has 0% chance of occuring)

		uint32_t scoreInt = node.score >> 32;
		float& score = reinterpret_cast<float&>(scoreInt);
		float chance = remainderChance;
		std::array<unsigned long, 2> wildcardCopies = { wildcards[wildcardValue == 0 ? 1 : 0], wildcards[wildcardValue == 2 ? 1 : 2] };
		for (int i = 0; i < 25; i++) {
			if (consumedWildcards & 0b1)
				chance /= (1 + (wildcardCopies[0] & 0b1) + (wildcardCopies[1] & 0b1));
			consumedWildcards >>= 1;
			wildcardCopies[0] >>= 1;
			wildcardCopies[1] >>= 1;
		}
		partialScore += chance * score;
		remainderChance -= chance;

		wildcards[wildcardValue] &= ~consumedWildcards;
		for (const auto& wildcard : wildcards)	// chance should be 0 by now but because of inaccuracies lets just check if any wildcards are left for use instead
			if (wildcard != 0)
				goto cnt;
		break;
		cnt:;
	}
	score = ((uint64_t)reinterpret_cast<uint32_t&>(partialScore) << 32) | (score & 0xffffffff);
}


ExploreNode::~ExploreNode() {
	// 127: vector was created
	// 126: vector was destructed again
	assert((info.board.data[127] == 0 && info.board.data[126] == 0 && info.board.data[125] == 0)
		|| (info.board.data[127] == 1 && info.board.data[126] == 1 && info.board.data[125] == 0));
}
