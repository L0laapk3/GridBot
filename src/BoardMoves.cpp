
#include "Board.h"
#include <iostream>
#include <algorithm>
#include <assert.h>

#include <initializer_list>



// only care about moves that contain MAXRAND or less wildcard cells.
constexpr int MAXRAND = 8;


void Board::iterateMoves(MoveFunc cb) const {
	//std::cout << std::endl;
	//print();
	auto foundMove = [&](const int& first, const int& second, const int& length, const Data& oldValue, const Move& cells, const int& cellShift, const int& numWildcards) {
		if (oldValue[4] == 0b1 && (length == 3 || length == 6))
			return;
		//std::cout << "found move of length " << length << ", first=" << first << ", second=" << second << ", cshift=" << cellShift << std::endl;
		//printRaw(expand(cells));
		assert(Bitset2::bitset2<25>(cells).count() == length);
		const int shiftAmount = first - cellShift;
		const Move move = shiftAmount > 0 ? cells << shiftAmount : cells >> -shiftAmount;
		//std::cout << "numwildcards " << numWildcards << " " << std::bitset<25>(shrink(andBits((data) == repeat(0x1f)))) << " " << std::bitset<25>(move) << std::endl;
		assert(numWildcards == Data(shrink(andBits((data) == repeat(0x1f))) & move).count());

		if (((oldValue & Data(0x1f)) ^ Data(0x0f)).any()) {
			const uint64_t wildcardValue = oldValue[4] ? 2 : oldValue[0];
			const Data shiftedCells = expand(move);
			short n = (short)(oldValue & Data(0x0f)).to_ulong();
			int l = length;
			while ((l & 0b1) == 0b0) {
				assert(l != 0);
				++n;
				l >>= 1;
			}
			assert(n < 16);
			Data newCell = Data(n);
			if (l == 3) {
				assert(oldValue[4] == 0b0);
				newCell[4] = 1;
			}
			else
				newCell[4] = oldValue[4];
			const Board board1 = Board(data & ~shiftedCells | (shiftedCells & ~cell(0b11111, 5 * first) & repeat(0b01111)) | newCell << (5 * first));
			const Board board2 = Board(data & ~shiftedCells | (shiftedCells & ~cell(0b11111, 5 * second) & repeat(0b01111)) | newCell << (5 * second));
			//board1.print();
			//std::cout << std::endl;
			//board2.print();
			//std::cout << std::bitset<25>(move) << " " << std::bitset<25>(cells) << std::endl;
			assert((Bitset2::bitset2<25>(move).count()) == length);
			cb(board1, move | (uint64_t)wildcardValue << 30 | (first << 25)); // lowest 25 bits are bitboard, next 5 bits are end position, last 2 bits are old cell value
			cb(board2, move | (uint64_t)wildcardValue << 30 | (second << 25));
		}
		else {
			//std::cout << "one with only wildcards" << std::endl;
			for (int wildcardValue = 0; wildcardValue < 3; wildcardValue++) {
				const auto& value = std::array<int, 3>{ 0x01, 0x02, 0x10 }[wildcardValue];
				const Data shiftedCells = expand(move);
				auto newCell = Data(value);
				const Board board1 = Board(data & ~shiftedCells | (shiftedCells & ~cell(0b11111, 5 * first) & repeat(0b01111)) | newCell << (5 * first));
				const Board board2 = Board(data & ~shiftedCells | (shiftedCells & ~cell(0b11111, 5 * second) & repeat(0b01111)) | newCell << (5 * second));
				assert((Bitset2::bitset2<25>(move).count()) == length);
				cb(board1, move | (uint64_t)wildcardValue << 30 | (first << 25));
				cb(board2, move | (uint64_t)wildcardValue << 30 | (second << 25));
			}
		}
	};



	constexpr auto randomGrid = repeat(0x0f);
	Data randomMask = maskedCompareToMask(data, randomGrid);

	std::function<void(std::array<Data, MAXRAND+1>&, Node const*, const int&, const int&, const Move&, const int&)> exploreCombinations =
		[&](std::array<Data, MAXRAND+1>& prevDatas, Node const* moves, const int& length, const int& end, const Move& cells, const int& cellShift) {
		auto moveOneDirection = [&](Node const* newMoves, const int& direction, const Data& mask) {
			if (newMoves == nullptr)
				return;
			std::array<Data, MAXRAND + 1> datas;
			for (int i = 0; i <= MAXRAND; i++)
				datas[i] = Data(0);
			Data prevToNextRandLevel = Data(0);
			const short newEnd = end - direction;
			const int newCellShift = direction < 0 ? cellShift : cellShift + direction;
			//std::cout << "dir=" << direction << ", cshift=" << cellShift << std::endl;
			const Move newCells = (direction < 0 ? cells << -direction : cells) | (1 << newCellShift);
			bool hasAtLeastOneResult = false;
			for (int i = 0; i <= std::min(length, MAXRAND); i++) {
				if (i < length) {
					const Data shifted = direction < 0 ? prevDatas[i] >> (-5 * direction) : prevDatas[i] << (5 * direction);
					const Data shiftedNoRandom = (shifted & ~randomMask) | prevToNextRandLevel;
					prevToNextRandLevel = shifted & randomMask;
					datas[i] = shiftedNoRandom & maskedCompareToMask(shiftedNoRandom, data, mask);
				} else {
					//std::cout << "moves consisting of only wildcards" << std::endl;
					datas[i] = repeat(0x0f) & maskedCompareToMask(prevToNextRandLevel, data, mask);
				}
				//printRaw(prevToNextRandLevel);

				if (datas[i].any()) {
					if (newMoves->end) {
						const Data finishedPath = datas[i] | datas[i] >> 1 | datas[i] >> 2 | datas[i] >> 3 | datas[i] >> 4;
						for (int j = 0; j < 25; j++)
							if (finishedPath[5 * j])
								foundMove(j, j + newEnd, length, (datas[i] >> (5 * j)), newCells, newCellShift, i);
					}
					hasAtLeastOneResult = true;
				}
			}
			if (hasAtLeastOneResult)
				exploreCombinations(datas, newMoves, length + 1, newEnd, newCells, newCellShift);
		};

		moveOneDirection(moves->left, -1, repeat(0x01, 1, 0, 0, 0));
		moveOneDirection(moves->up,   -5, repeat(0x01, 0, 1, 0, 0));
		moveOneDirection(moves->right, 1, repeat(0x01, 0, 0, 1, 0));
		moveOneDirection(moves->down,  5, repeat(0x01, 0, 0, 0, 1));
	};

	std::array<Data, MAXRAND+1> datas;
	datas[0] = data & ~randomMask;
	if (MAXRAND >= 1)
		datas[1] = randomMask;
	for (int i = 2; i <= MAXRAND; i++)
		datas[i] = Data(0);
	exploreCombinations(datas, &ALLMOVES, 2, 0, 0b1, 0);

}