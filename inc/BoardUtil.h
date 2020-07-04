#pragma once


#include <immintrin.h>
#include <iostream>


const void printRaw(const Data& data);



constexpr Data raw(
	const unsigned int& c00 = 0, const unsigned int& c01 = 0, const unsigned int& c02 = 0, const unsigned int& c03 = 0, const unsigned int& c04 = 0,
	const unsigned int& c10 = 0, const unsigned int& c11 = 0, const unsigned int& c12 = 0, const unsigned int& c13 = 0, const unsigned int& c14 = 0,
	const unsigned int& c20 = 0, const unsigned int& c21 = 0, const unsigned int& c22 = 0, const unsigned int& c23 = 0, const unsigned int& c24 = 0,
	const unsigned int& c30 = 0, const unsigned int& c31 = 0, const unsigned int& c32 = 0, const unsigned int& c33 = 0, const unsigned int& c34 = 0,
	const unsigned int& c40 = 0, const unsigned int& c41 = 0, const unsigned int& c42 = 0, const unsigned int& c43 = 0, const unsigned int& c44 = 0) {
	return Data(c00) | Data(c01) << 5 | Data(c02) << 10 | Data(c03) << 15 | Data(c04) << 20
		| Data(c10) << 25 | Data(c11) << 30 | Data(c12) << 35 | Data(c13) << 40 | Data(c14) << 45
		| Data(c20) << 50 | Data(c21) << 55 | Data(c22) << 60 | Data(c23) << 65 | Data(c24) << 70
		| Data(c30) << 75 | Data(c31) << 80 | Data(c32) << 85 | Data(c33) << 90 | Data(c34) << 95
		| Data(c40) << 100 | Data(c41) << 05 | Data(c42) << 110 | Data(c43) << 115 | Data(c44) << 120;
}

constexpr Data repeatRow(const unsigned long& row, const int& down = 0, const int& up = 0) {
	return (down <= 4 && up <= 0 ? Data(row) : Data(0)) |
		(down <= 3 && up <= 1 ? Data(row) << 25 : Data(0)) |
		(down <= 2 && up <= 2 ? Data(row) << 50 : Data(0)) |
		(down <= 1 && up <= 3 ? Data(row) << 75 : Data(0)) |
		(down <= 0 && up <= 4 ? Data(row) << 100 : Data(0));
}

constexpr Data repeat(const unsigned int& value, const int& right = 0, const int& down = 0, const int& left = 0, const int& up = 0) {
	const unsigned long row =
		(right <= 4 && left <= 0 ? value : 0) |
		(right <= 3 && left <= 1 ? value << 5 : 0) |
		(right <= 2 && left <= 2 ? value << 10 : 0) |
		(right <= 1 && left <= 3 ? value << 15 : 0) |
		(right <= 0 && left <= 4 ? value << 20 : 0);
	return repeatRow(row, down, up);
}

constexpr Data repeatCol(const unsigned int& r0, const unsigned int& r1 = 0, const unsigned int& r2 = 0, const unsigned int& r3 = 0, const unsigned int& r4 = 0, const int& right = 0, const int& left = 0) {
	const Data col = Data(r0) | Data(r1) << 25 | Data(r2) << 50 | Data(r3) << 75 | Data(r4) << 100;
	return
		(right <= 4 && left <= 0 ? col : Data(0)) |
		(right <= 3 && left <= 1 ? col << 5 : Data(0)) |
		(right <= 2 && left <= 2 ? col << 10 : Data(0)) |
		(right <= 1 && left <= 3 ? col << 15 : Data(0)) |
		(right <= 0 && left <= 4 ? col << 20 : Data(0));
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