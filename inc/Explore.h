#pragma once

#include <vector>

#include "Board.h"
#include "ScoreSortable.h"

constexpr auto MS_PER_ACTION = 400;
constexpr auto RESERVE_SIZE = 300 * MS_PER_ACTION;

class ExploreNode;
struct Candidate : public ScoreSortable {
	ExploreNode* node;
};

std::vector<int> findBestMove(const Board& board);
