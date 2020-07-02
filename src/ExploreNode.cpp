
#include "exploreNode.h"

#include <cassert>

ExploreNode::ExploreNode(const Board& board, const uint64_t& score) : info{ board }, score(score) {
	assert(info.board.data[127] == 0 && info.board.data[126] == 0 && info.board.data[125] == 0);
}

ExploreNode::ExploreNode(const ExploreNode& exploreNode) : score(exploreNode.score) {
	if (info.board.data[127])
		info.deep = exploreNode.info.deep;
	else
		info.board = exploreNode.info.board;
}

std::vector<ExploreNode>& ExploreNode::Explore(const uint64_t& prevScore) {
	// will use the consumed wildcards in the lower 32 bits of prevScore
#ifndef NDEBUG
	assert(info.board.data[127] == 0 && info.board.data[126] == 0 && info.board.data[125] == 0);
#endif

	bool first = false;
	MoveFunc foundMove = [&](const Board& board, const unsigned long& consumedWildcards) {
		if (!first) {
			first = true;
			// at this point, we switch union to deep
			info.deep = { shrink(andBits(info.board.data == repeat(0x0f))) << 32 | (prevScore & 0xffffffff) | 1 << 63, { } };
		}
		const float& score = board.eval();
		info.deep.childNodes.push_back(ExploreNode(board, (reinterpret_cast<const uint32_t&>(score) << 32) | consumedWildcards));
	};
	info.board.iterateMoves(foundMove);

	info.deep.childNodes.shrink_to_fit();

	return info.deep.childNodes;
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
	for (ExploreNode& node : info.deep.childNodes)
		node.computeScore();

	std::array<unsigned long, 3> wildcards{ info.deep.wildcardInfo >> 32 };
	float partialScore = 0;
	float remainderChance = 1;
	for (auto i = info.deep.childNodes.begin(); i < info.deep.childNodes.end(); i++) {
		std::partial_sort(i, i + 1, info.deep.childNodes.end(), [](const ExploreNode& a, const ExploreNode& b) { return a.score > b.score; });
		const ExploreNode& node = *i;
		unsigned long consumedWildcards = node.score;
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
	score = (reinterpret_cast<uint32_t&>(partialScore) << 32) | (score & 0xffffffff);

	info.deep.childNodes.~vector();
	return;
}

ExploreNode::~ExploreNode() {
	assert((info.board.data[127] == 0 && info.board.data[126] == 0 && info.board.data[125] == 0)
		|| (info.board.data[127] == 1 && info.board.data[126] == 1 && info.board.data[125] == 0));
}
