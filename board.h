//
// Created by artyom-m on 3/19/19.
//
#pragma once

#include <string>
#include <iostream>

#include "types.h"
#include "engine.h"

using namespace std;

class EPD {
public:

    EPD(const string& s);
    void print();

private:
    bitboard wKings, wQueens, wRooks, wBishops, wKnights, wPawns;
    bitboard bKings, bQueens, bRooks, bBishops, bKnights, bPawns;

    void reset();
};

void init(EPD position);

