//
// Created by artyom-m on 4/15/19.
//

#include "constants.h"
#include "iostream"

namespace Constants {

    void init_queens() {

        for (int i = 0; i < 64; i++) {
            KING_MOVES[i] = 0;
            if (i % 8 != 0)
                KING_MOVES[i] += 1ull << (i - 1);
            if (i % 8 != 7)
                KING_MOVES[i] += 1ull << (i + 1);
            if (i / 8 != 0)
                KING_MOVES[i] += 1ull << (i - 8);
            if (i / 8 != 7)
                KING_MOVES[i] += 1ull << (i + 8);

            if (i % 8 != 0 && i / 8 != 0)
                KING_MOVES[i] += 1ull << (i - 9);
            if (i % 8 != 0 && i / 8 != 7)
                KING_MOVES[i] += 1ull << (i + 7);
            if (i % 8 != 7 && i / 8 != 0)
                KING_MOVES[i] += 1ull << (i - 7);
            if (i % 8 != 7 && i / 8 != 7)
                KING_MOVES[i] += 1ull << (i + 9);

            std::cout << KING_MOVES[i] << ", ";
            if (i%8 == 7) std::cout << "\n";

        }

    }

    void init() {
//        init_queens();
    }
}
