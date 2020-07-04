#pragma once


#include <immintrin.h>
#include <iostream>




constexpr Data repeat(const unsigned int& valueInt, const int& right = 0, const int& down = 0, const int& left = 0, const int& up = 0) {
	const Data value = Data(valueInt);
	const Data row =
		(right <= 4 && left <= 0 ? value : Data(0)) |
		(right <= 3 && left <= 1 ? value << 5 : Data(0)) |
		(right <= 2 && left <= 2 ? value << 10 : Data(0)) |
		(right <= 1 && left <= 3 ? value << 15 : Data(0)) |
		(right <= 0 && left <= 4 ? value << 20 : Data(0));
	return
		(down <= 4 && up <= 0 ? row : Data(0)) |
		(down <= 3 && up <= 1 ? row << 25 : Data(0)) |
		(down <= 2 && up <= 2 ? row << 50 : Data(0)) |
		(down <= 1 && up <= 3 ? row << 75 : Data(0)) |
		(down <= 0 && up <= 4 ? row << 100 : Data(0));
}
constexpr Data cell(const unsigned int& valueInt, const short& at) {
	const Data value = Data(valueInt) & Data(0b11111);
	return value << at;
}
constexpr Data mask(const unsigned long long& bits, const unsigned int& valueInt) {
	const Data value = Data(valueInt) & Data(0b11111);
	return
		(bits & 2 << 0 ? value << 0 : Data(0)) &
		(bits & 2 << 1 ? value << 5 : Data(0)) &
		(bits & 2 << 2 ? value << 10 : Data(0)) &
		(bits & 2 << 3 ? value << 15 : Data(0)) &
		(bits & 2 << 4 ? value << 20 : Data(0)) &
		(bits & 2 << 5 ? value << 25 : Data(0)) &
		(bits & 2 << 6 ? value << 30 : Data(0)) &
		(bits & 2 << 7 ? value << 35 : Data(0)) &
		(bits & 2 << 8 ? value << 40 : Data(0)) &
		(bits & 2 << 9 ? value << 45 : Data(0)) &
		(bits & 2 << 10 ? value << 50 : Data(0)) &
		(bits & 2 << 11 ? value << 55 : Data(0)) &
		(bits & 2 << 12 ? value << 60 : Data(0)) &
		(bits & 2 << 13 ? value << 65 : Data(0)) &
		(bits & 2 << 14 ? value << 70 : Data(0)) &
		(bits & 2 << 15 ? value << 75 : Data(0)) &
		(bits & 2 << 16 ? value << 80 : Data(0)) &
		(bits & 2 << 17 ? value << 85 : Data(0)) &
		(bits & 2 << 18 ? value << 90 : Data(0)) &
		(bits & 2 << 19 ? value << 95 : Data(0)) &
		(bits & 2 << 20 ? value << 100 : Data(0)) &
		(bits & 2 << 21 ? value << 105 : Data(0)) &
		(bits & 2 << 22 ? value << 110 : Data(0)) &
		(bits & 2 << 23 ? value << 115 : Data(0)) &
		(bits & 2 << 24 ? value << 120 : Data(0));
}

constexpr Data andBits(Data const& a) {
	return a & a >> 1 & a >> 2 & a >> 3 & a >> 4;
}

constexpr Data maskedCompare(Data const& a, Data const& b, Data const& mask = repeat(0x01)) {
	const Data c = a == b;
	const Data cumul = andBits(c) & mask;
	return cumul;
}

constexpr Data toMask(Data const& a) {
	return a | a << 1 | a << 2 | a << 3 | a << 4;
}

constexpr Data maskedCompareToMask(Data const& a, Data const& b, Data const& mask = repeat(0x01)) {
	return toMask(maskedCompare(a, b, mask));
}

const Data expand(const Move& move, const unsigned int& valueInt = 0b11111);
const uint64_t shrink(const Data& data);

const void printRaw(const Data& data);