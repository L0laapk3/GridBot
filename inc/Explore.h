#pragma once

#include <vector>

#include "Board.h"


class ExploreNode;
struct Candidate {
	uint64_t score;
	ExploreNode* node;
	bool operator<(const Candidate& other) const { return score < other.score; };
	bool operator>(const Candidate& other) const { return score > other.score; };
};

// seems to be faster than manual sorting for > ~.5 sec computation time. better scaling. no worrying about deleting relevant parts of Candidates.
typedef std::vector<Candidate> Candidates;

unsigned long findBestMove(const Board& board);
