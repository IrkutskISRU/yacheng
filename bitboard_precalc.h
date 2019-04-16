//
// Created by artyom-m on 4/16/19.
//

#pragma once
#include "types.h"
#include "precalc.h"

namespace BitBoardPrecalc {
    const vector <vector <bitboard> > hor = Precalc::precalc_hor();
    const vector <vector <bitboard> > ver = Precalc::precalc_ver();
    static bitboard dia1[64][256];
    static bitboard dia2[64][256];

    const vector <int> to_ver = Precalc::precalc_to_ver();

}