#pragma once

#include <array>

class Board {
public:
	void set(std::array<unsigned long, 25>& value);
	void print();
	float eval();

private:
	std::array<unsigned long, 25> data;
};