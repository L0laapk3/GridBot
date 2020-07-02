#pragma once

#include <functional>

#include "bitset2/bitset2.hpp"

#include "MovesTree.h"
#include <vector>



typedef Bitset2::bitset2<128> Data;
typedef uint32_t Move;
typedef float Score;

class Board;
typedef std::function<void(const Board&, const Move&)> MoveFunc;


#include "BoardUtil.h"



void printRaw(const Data& data);

class Board {
public:
	Board();
	Board(const Data& data);
	Board(const Board& board);
	~Board();

	uint32_t valueAt(const int& pos) const;

	void print() const;
	void set(const std::string& str);

	uint32_t findBestMove() const;
	void iterateMoves(MoveFunc cb) const;

	std::vector<int> decompressMove(const Move& move) const;

	const float& eval() const;


	Data data = Data(0);

};

