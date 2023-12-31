//
// Created by artyom-m on 4/16/19.
//

#pragma once
#include "types.h"
#include "precalc.h"

namespace BitBoardPrecalc {
    const vector <vector <bitboard> > hor = Precalc::precalc_hor();
    const vector <vector <bitboard> > ver = Precalc::precalc_ver();
    const vector <vector <bitboard> > dia1 = Precalc::precalc_dia1();
    const vector <vector <bitboard> > dia2 = Precalc::precalc_dia2();

    const vector <int> to_ver = Precalc::precalc_to_ver();
    const vector <int> to_dia1 = Precalc::precalc_to_dia1();
    const vector <int> to_dia2 = Precalc::precalc_to_dia2();

}