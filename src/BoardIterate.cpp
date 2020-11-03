#include "Board.h"

#include <functional>
#include <cassert>
#include <iostream>

struct Position {
    unsigned long x;
    unsigned long y;
};

void Board::iterateMoves(MoveFunc cb) const {
    for (unsigned long i = 0; i < 25; i++) {
        unsigned long visited = 1 << i;
        unsigned long length = 1;
        auto newBoard = Board(*this);
        for (auto value = data[i]; value < (data[i] ? data[i] + 1 : 3); value++) {
            std::function<void(const unsigned long&)> tryAdd = [&](const unsigned long& p) {
                const auto bit = 1 << p;
                if (data[p] == value && (~visited & bit)) {
                    visited |= bit;
                    length++;
                    newBoard.data[p] = length * value;
                    cb(newBoard, visited, p, value);

                    newBoard.data[p] = value;
                    if (p % 5 != 4) tryAdd(p + 1);
                    if (p % 5 != 0) tryAdd(p - 1);
                    if (p < 20) tryAdd(p + 5);
                    if (p >= 5) tryAdd(p - 5);

                    length--;
                    newBoard.data[p] = data[p];
                    visited &= ~bit;
                }
            };

            newBoard.data[i] = value;
            if (i % 5 != 4) tryAdd(i + 1);
            if (i % 5 != 0) tryAdd(i - 1);
            if (i < 20) tryAdd(i + 5);
            if (i >= 5) tryAdd(i - 5);
        }
    }
}