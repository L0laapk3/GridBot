#include "Board.h"
#include <iostream>
#include <bitset>
#include <cassert>
#include <array>



Board::Board() {
}




// every cell is 5 bits: mnnnn
// 4 smallest bits are for n
// largest bit is for m
// value is 2^n * 3^m
// m=0 and n=1111 -> random




Board::Board(const Data& data) {
	this->data = data;
}


Board::Board(const Board& board) : data(board.data) {};

uint32_t Board::valueAt(const int& pos) const {
	unsigned long n = ((data >> pos)& Data(0b1111)).to_ulong();
	unsigned long m = ((data >> (pos + 4))& Data(0b1)).to_ulong();
	return (1 << (n - 1)) * (m ? 3 : 1);
}


void Board::print() const {
	int i = 0;
	for (int j = 0; j < 5; j++) {
		for (int k = 0; k < 5; k++) {
			unsigned long n = ((data >> i) & Data(0b1111)).to_ulong();
			unsigned long m = ((data >> (i + 4)) & Data(0b1)).to_ulong();
			if (m == 0b0 && n == 0b1111)
				std::cout << "  ?";
			else if (m == 0b0 && n == 0b0)
				std::cout << "  _";
			else {
				int value = (1 << (n - 1)) * (m ? 3 : 1);
				if (value < 100)
					std::cout << " ";
				if (value < 10)
					std::cout << " ";
				std::cout << value;
			}
			//if (k < 4)
			//	std::cout << " ";
			i += 5;
		}
		std::cout << std::endl;	
	}
}


void printRaw(const Data& data) {
	unsigned long i = 0;
	for (int j = 0; j < 5; j++) {
		for (int k = 0; k < 5; k++) {
			for (int m = 0; m < 5; m++) {
				std::cout << ((data >> i) & Data(0b1)).to_ulong();
				i++;
			}
			if (k < 4)
				std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


void Board::set(const std::string& str) {
	data.reset();
	int i = 0;
	int j = 1;
	int value = 0;
	while (i < 125) {
		if (isdigit(str[j]))
			value = 10 * value + ((int)str[j] - 48);
		else {
			unsigned __int64 m = value % 3 == 0;
			if (m)
				value /= 3;
			unsigned __int64 n = 1;
			int N = 1;
			while (N < value) {
				++n;
				N = N << 1;
			}
			if (N != value || n > 15) {
				std::cout << "value is not 2^N * 3^M: " << value << std::endl;
				exit(1);
			}

			if (((data >> i) & Data(0x1f)).any()) {
				if(((data >> i) & Data(0x1f) ^ Data((n + (m << 4)))).any()) {
					std::cout << "cell mismatch" << std::endl;
					exit(1);
				}
			}
			data |= Data((n + (m << 4))) << i;

			i += 5;
			value = 0;
		}
		++j;
	}
}



std::vector<int> Board::decompressMove(const Move& move) const {
	//print();
	int pos = move >> 25;
	const Bitset2::bitset2<25> bitboard(move & 0x1ffffff);
	int length = bitboard.count();
	const Data value = (data >> (5 * pos)) & Data(0x1f);

	auto v = std::vector<int>{ pos };

	std::function<bool()> tryDirections = [&]() {
		for (const int direction : std::array<int, 4>{ -5, -1, 1, 5 }) {
			const int newPos = pos + direction;
			if (newPos < 0 || newPos >= 25 || (std::abs(direction) == 1 && pos / 5 != newPos / 5) || std::find(v.begin(), v.end(), newPos) != v.end())
				continue;
			if (bitboard[newPos] == 0)
				continue;
			pos = newPos;
			assert(~(value ^ ((data >> (5 * pos)) & Data(0x1f))).any());
			v.push_back(pos);
			if (v.size() >= length)
				return true;
			if (tryDirections())
				return true;
			v.pop_back();
			pos -= direction;
		}
		return false;
	};
	tryDirections();

	assert(v.size() == length);

	std::reverse(v.begin(), v.end());
	return v;
}