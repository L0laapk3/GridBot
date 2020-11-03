#include "Board.h"



float Board::computeScore(unsigned long remainingDepth) const {
	if (remainingDepth <= 0)
		return eval();
	struct Child {
		float score;
		Move move;
		Value wildcardValue;
	};

	unsigned long wildcards = 0UL;
	for (size_t i = 0; i < 25; i++)
		if (data[i] == 0)
			wildcards |= 1 << i;
	std::array<unsigned long long, 3> remainingWildcards = { wildcards, wildcards, wildcards };

	std::vector<Child> children;
	iterateMoves([&](const Board& board, const Move& move, const Value wildcardValue) {
		children.push_back({
			board.computeScore(remainingDepth - 1),
			move,
			wildcardValue
		});
	});

	float partialScore = 0.f;
	float remainderChance = 1.f;
	for (auto it = children.begin(); it < children.end(); it++) {
		std::partial_sort(it, it + 1, children.end(), [](const Child& a, const Child& b) { return a.score > b.score; });

		float chance = remainderChance;
		unsigned long consumedWildcards = 0UL;
		for (size_t i = 0; i < 25; i++)
			if (data[i] != 0)
				consumedWildcards |= 1 << i;
		consumedWildcards &= wildcards;

		if (consumedWildcards) {
			if (consumedWildcards & ~remainingWildcards[it->wildcardValue])
				continue; // a wildcard is required that is already used by a higher scoring move
			std::array<unsigned long, 2> wildcardCopies = { remainingWildcards[it->wildcardValue == 0 ? 1 : 0], remainingWildcards[it->wildcardValue == 2 ? 1 : 2] };
			for (int i = 0; i < 25; i++) {
				if (consumedWildcards & 0b1)
					chance /= (1.f + (wildcardCopies[0] & 0b1) + (wildcardCopies[1] & 0b1));
				consumedWildcards >>= 1;
				wildcardCopies[0] >>= 1;
				wildcardCopies[1] >>= 1;
			}
		}

		partialScore += chance * it->score;

		if (chance == remainderChance) // no more wildcards remaining, all other moves are worse and a have 0% chance of being used
			break;
		remainderChance -= chance;
		remainingWildcards[it->wildcardValue] &= ~consumedWildcards;
	}

	return partialScore;
}

Move Board::depthFirstSearch(unsigned long depth) const {
	struct Action {
		float score;
		Move move;
	};
	Action bestAction{ 0 };
	iterateMoves([&](const Board& board, const Move& move, const Value wildcardValue) {
		const float score = board.computeScore(depth - 1);
		if (score > bestAction.score) {
			bestAction.score = score;
			bestAction.move = move;
		}
		});
	return bestAction.move;
}


Move Board::findBestMove(unsigned long timeBudget) const {
	return depthFirstSearch(2);
}