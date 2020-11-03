#include "Explore.h"


float computeScore(const Board& board, unsigned long remainingDepth) {
	if (remainingDepth <= 0)
		return board.eval();
	struct Child {
		float score;
		Move move;
		Value usedValue;
	};

	Move wildCards = board.data
	std::vector<Child> children;
	board.iterateMoves([&](const Board& board, const Move& move, const Value usedValue) {
		children.push_back({
			computeScore(board, remainingDepth - 1),
			move,
			usedValue
		});
	});
	return 0.f;
}

Move depthFirstSearch(const Board& board, unsigned long depth) {
	struct Action {
		float score;
		Move move;
	};
	Action bestAction{ 0 };
	board.iterateMoves([&](const Board& board, const Move& move, const Value usedValue) {
		const float score = computeScore(board, depth - 1);
		if (score > bestAction.score) {
			bestAction.score = score;
			bestAction.move = move;
		}
	});
	return bestAction.move;
}


Move findBestMove(const Board& board, unsigned long timeBudget) {
	return depthFirstSearch(board, 2);
}