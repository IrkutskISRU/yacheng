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

}

