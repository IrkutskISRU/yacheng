//
// Created by artyom-m on 4/15/19.
//
#pragma once
#include "types.h"

namespace BitBoard {

    const ull MASK = 65535;
    static int decode[50000];

    bitboard print(bitboard bb);
    int bitNumberFromBitBoard(bitboard bb);
    void initDecode();

}