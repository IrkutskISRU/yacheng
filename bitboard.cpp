//
// Created by artyom-m on 4/15/19.
//

#include "bitboard.h"
#include <iostream>

namespace BitBoard {

    void print(bitboard bb) {
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

    bitboard conv_to_ver(bitboard x)
    {
        ull x2 = 0;
        for (int i = 0; i < 64; i++)
        if (x & (1ull << i)) x2 += (1ull << BitBoardPrecalc::to_ver[i]);
        return x2;
    }
    bitboard conv_to_dia1(bitboard x) {
        ull x2 = 0;
        for (int i = 0; i < 64; i++)
            if (x & (1ull << i)) x2 += (1ull << BitBoardPrecalc::to_dia1[i]);
        return x2;
    }
    bitboard conv_to_dia2(bitboard x) {
        ull x2 = 0;
        for (int i = 0; i < 64; i++)
            if (x & (1ull << i)) x2 += (1ull << BitBoardPrecalc::to_dia2[i]);
        return x2;
    }
}

