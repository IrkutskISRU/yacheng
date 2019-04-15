//
// Created by artyom-m on 3/19/19.
//
#pragma once

#include <string>
#include <iostream>

#include "types.h"
#include "constants.h"

using namespace std;

struct EPD {
    EPD();
    EPD(const string& s);
    void print() const;
    bitboard wKings, wQueens, wRooks, wBishops, wKnights, wPawns;
    bitboard bKings, bQueens, bRooks, bBishops, bKnights, bPawns;
    int color;
    bool castleQ, castleK, castleq, castlek;
    string enPassant;
    void reset();
};

