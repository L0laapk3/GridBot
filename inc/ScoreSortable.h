#pragma once

#include <stdint.h>



class ScoreSortable {
public:
	uint64_t score;

	const float getScore() const {
		unsigned long s = score >> 32;
		return *(float*)&s;
	}

	bool operator<(const ScoreSortable& other) const { return *(double*)&score < *(double*)&other.score; };
	bool operator>(const ScoreSortable& other) const { return *(double*)&score > *(double*)&other.score; };
};