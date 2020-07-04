

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
		//std::cout << (childNodes.back().score >> 32) << " " << std::bitset<25>(move & availableWildcards) << std::endl;
	});

	childNodes.shrink_to_fit();

	for (ExploreNode& node : childNodes) {
		assert(node.info.board.data[127] == 0);
		candidates.push_back(Candidate{ node.score, &node });
	}
	for (const auto& candidate : candidates)
		assert(candidate.node->info.board.data[127] == 0);

	//std::cout << std::bitset<25>(availableWildcards) << std::endl;
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
		it->computeScore();


	//std::cout << "read " << std::bitset<25>(info.childNodes.wildcards_size >> 32) << std::endl;
	const unsigned long wildcards = info.childNodes.wildcards_size >> 32 & 0x1ffffff;
	std::array<unsigned long, 3> wildcardsLeft = { wildcards, wildcards, wildcards };

	float partialScore = 0.f;
	float remainderChance = 1.f;
	for (auto it = info.childNodes.begin; it < end; it++) {
		std::partial_sort(it, it + 1, end, [](const ExploreNode& a, const ExploreNode& b) { return a.score > b.score; });

		unsigned long consumedWildcards = it->score & wildcards;
		//std::cout << std::bitset<25>(it->score) << " " << std::bitset<25>(~it->score & wildcards) << " " << std::bitset<25>(consumedWildcards) << std::endl;
		const unsigned short wildcardValue = (it->score >> 30) & 0x3;	// 00 = 2, 01 = 1, 10 = 3
		uint32_t scoreInt = it->score >> 32;
		float& subScore = *(float*)(&scoreInt);
		it->~ExploreNode();
		float chance = remainderChance;
		if (consumedWildcards) {
			std::cout << std::bitset<25>(consumedWildcards) << std::endl;
			if (consumedWildcards & ~wildcardsLeft[wildcardValue]) // check if a wildcard is used that is no longer available.
				continue;
			std::array<unsigned long, 2> wildcardCopies = { wildcardsLeft[wildcardValue == 0 ? 1 : 0], wildcardsLeft[wildcardValue == 2 ? 1 : 2] };
			for (int i = 0; i < 25; i++) {
				std::cout << i << std::endl << std::bitset<25>(wildcards) << std::endl << std::bitset<25>(consumedWildcards) << std::endl << std::endl << std::bitset<25>(wildcardCopies[0]) << std::endl << std::bitset<25>(wildcardCopies[1]) << std::endl << std::endl;

				//std::cout << i << " " << consumedWildcards << std::endl;
				if (consumedWildcards & 0b1) {
					std::cout << chance << " " << (wildcardCopies[0] & 0b1) << (wildcardCopies[1] & 0b1) << std::endl;
					chance /= (1.f + (wildcardCopies[0] & 0b1) + (wildcardCopies[1] & 0b1));
				}
				consumedWildcards >>= 1;
				wildcardCopies[0] >>= 1;
				wildcardCopies[1] >>= 1;
			}
			std::cout << "subscore " << subScore << " " << chance << std::endl;
		}

		if (chance == remainderChance) {
			partialScore += subScore;
			break;
		}

		partialScore += chance * subScore;
		remainderChance -= chance;

		wildcardsLeft[wildcardValue] &= ~consumedWildcards;
		for (const auto& wildcard : wildcardsLeft)	// remainderChance should be 0 by now but because of inaccuracies lets just check if any wildcards are left for use instead
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
