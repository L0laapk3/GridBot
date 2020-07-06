#pragma once

#include <functional>

#include "bitset2/bitset2.hpp"

#include "MovesTree.h"
#include <vector>


typedef Bitset2::bitset2<128> Data;

#include "BoardUtil.h"


class Board;
typedef std::function<void(const Board&, const unsigned long& move, const unsigned long& wildcardValue, const unsigned long endPosition)> MoveFunc;


class ExploreNode;
class Board {
public:
	Board();
	Board(const Data& data);
	Board(const Board& board);
	~Board();

	uint32_t valueAt(const int& pos) const;

	void print() const;
	void set(const std::string& str);

	void iterateMoves(MoveFunc cb) const;

	std::vector<unsigned long> decompressMove(const unsigned long& move, const unsigned long& endPos) const;

	const float eval() const;


	Data data = Data(0);

};

