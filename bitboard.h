//
// Created by artyom-m on 4/15/19.
//
#pragma once
#include "types.h"
#include "bitboard_precalc.h"

namespace BitBoard {

    const ull MASK = 65535;
    static int decode[50000];

    void print(bitboard bb);
    int bitNumberFromBitBoard(bitboard bb);
    void initDecode();

    bitboard conv_to_ver(bitboard x);
    bitboard conv_to_dia1(bitboard x);
    bitboard conv_to_dia2(bitboard x);

}
