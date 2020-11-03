#include "Board.h"

#include <iostream>

void Board::set(std::array<unsigned long, 25>& value) {
	data = value;
}

void Board::print() {
	std::array<unsigned long, 25> values = data;
	for (size_t i = 0; i < 25; i++)
		std::cout << values[i] << (i % 5 == 4 ? '\n' : ' ');
}