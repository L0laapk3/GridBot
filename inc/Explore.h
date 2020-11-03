#pragma once

#include "Board.h"

Move findBestMove(const Board& board, unsigned long timeBudget);
Move depthFirstSearch(const Board& board, unsigned long depth);