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

// seems to be faster than manual sorting for > ~.5 sec computation time. better scaling. no worrying about deleting relevant parts of Candidates.
typedef std::vector<Candidate> Candidates;

std::vector<int> findBestMove(const Board& board);
