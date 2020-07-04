#pragma once

#include <functional>

#include "bitset2/bitset2.hpp"

#include "MovesTree.h"
#include <vector>


typedef Bitset2::bitset2<128> Data;
typedef unsigned long Move;

#include "BoardUtil.h"




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

	void iterateMoves(std::vector<ExploreNode>& childNodes) const;

	std::vector<int> decompressMove(const Move& move) const;

	const float eval() const;


	Data data = Data(0);

};

