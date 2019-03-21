//
// Created by artyom-m on 3/19/19.
//
#pragma once

#include <string>
#include <iostream>

#include "types.h"
#include "constants.h"

using namespace std;

class EPD {
public:

    EPD(const string& s);
    void print() const;

private:
    bitboard wKings, wQueens, wRooks, wBishops, wKnights, wPawns;
    bitboard bKings, bQueens, bRooks, bBishops, bKnights, bPawns;
    int color;

    void reset();
};

