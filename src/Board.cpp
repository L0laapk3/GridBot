#include "Board.h"

#include <cassert>
#include <iostream>
#include <bitset>


void Board::set(Data& value) {
	data = value;
}
Value Board::get(const Value cell) {
	return data[cell];
}

void Board::print() const {
	Data values = data;
	for (size_t i = 0; i < 25; i++) {
		if (values[i] < 10)
			std::cout << " ";
		std::cout << values[i];
		std::cout << (i % 5 == 4 ? '\n' : ' ');
	}
}

Move Board::decompressMove(CompactMove move, const Value endPos) const {
	unsigned long pos = endPos;
	const Value value = data[endPos];
	move &= ~(1 << endPos);
	auto v = std::vector<unsigned long>{ pos };

	std::function<bool(const unsigned long&)> tryAdd = [&](const unsigned long& p) {
		const auto bit = 1 << p;
		if (move & bit) {
			assert(data[p] == value);
			v.push_back(p);
			move &= ~bit;
			if (!move) {
				std::reverse(v.begin(), v.end());
				return true;
			}

			if (p % 5 != 4 && tryAdd(p + 1)) return true;
			if (p % 5 != 0 && tryAdd(p - 1)) return true;
			if (p < 20 && tryAdd(p + 5)) return true;
			if (p >= 5 && tryAdd(p - 5)) return true;

			move |= bit;
			v.pop_back();
		}
		return false;
	};

	if (endPos % 5 != 4 && tryAdd(endPos + 1)) return v;
	if (endPos % 5 != 0 && tryAdd(endPos - 1)) return v;
	if (endPos < 20 && tryAdd(endPos + 5)) return v;
	if (endPos >= 5 && tryAdd(endPos - 5)) return v;

	throw "cannot decompress";
}