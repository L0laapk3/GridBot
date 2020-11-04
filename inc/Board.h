#pragma once

#include <array>
#include <stack>
#include <functional>


typedef unsigned long Value;
typedef unsigned long CompactMove;
typedef std::vector<Value> Move;
typedef std::array<Value, 25> Data;

class Board;
typedef std::function<void(const Board& board, const CompactMove move, const Value endPos, const Value value)> MoveFunc;

class Board {
public:
	//board.cpp
	void set(Data& value);
	Value get(const Value cell);
	void print() const;
	Move decompressMove(CompactMove move, const Value endPos) const;

	//boardEval.cpp
	float eval() const;

	//boardIterate.cpp
	void iterateMoves(MoveFunc) const;

	//boardSearch.cpp
	Move findBestMove(unsigned long timeBudget) const;
	Move depthFirstSearch(unsigned long depth) const;
private:
	float computeScore(unsigned long remainingDepth) const;

private:
	Data data;
};