#include "Board.h"

#include <iostream>

void Board::set(Data& value) {
	data = value;
}

void Board::print() const {
	Data values = data;
	for (size_t i = 0; i < 25; i++)
		std::cout << values[i] << (i % 5 == 4 ? '\n' : ' ');
}