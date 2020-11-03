#pragma once

#include <array>
#include <functional>

typedef unsigned long Value;
typedef std::vector<Value> Move;
typedef std::array<Value, 25> Data;

class Board;
typedef std::function<void(const Board&, const Move&, const Value)> MoveFunc;

class Board {
public:
	//board.cpp
	void set(Data& value);
	void print() const;

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