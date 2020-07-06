#pragma once

#include <vector>

#include "Board.h"
#include "ScoreSortable.h"

class ExploreNode;
struct Candidate : public ScoreSortable {
	ExploreNode* node;
};

std::vector<int> findBestMove(const Board& board, long timeBudget);
