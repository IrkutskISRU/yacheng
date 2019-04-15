//
// Created by artyom-m on 4/15/19.
//

#include "bitboard.h"
#include <iostream>

namespace BitBoard {

    bitboard print(bitboard bb) {
        for (ull i = 0; i < 64; i++) {
            if (bb & (1ull << i)) {
                std::cout << "*";
            } else {
                std::cout << ".";
            }
            if (i % 8 == 7) {
                std::cout << "\n";
            }
        }


    }

    void initDecode() {
        int i = 1;
        for (int j = 0; j <= 15; j++) {
            decode[i] = j;
            i *= 2;
        }
    };

    int bitNumberFromBitBoard(bitboard bb) {

        ull ans = 0;

        ans = (bb >> 48) & MASK;
        if (ans) return decode[ans] + 48;

        ans = (bb >> 32) & MASK;
        if (ans) return decode[ans] + 32;

        ans = (bb >> 16) & MASK;
        if (ans) return decode[ans] + 16;

        return decode[bb & MASK];

    }

}

