//
// Created by artyom-m on 4/16/19.
//

#include <vector>
using namespace std;

namespace Precalc {
    vector <vector <bitboard> > precalc_hor();
    vector <vector <bitboard> > precalc_ver();
    vector <vector <bitboard> > precalc_dia1();
    vector <vector <bitboard> > precalc_dia2();

    vector <int> precalc_to_ver();
    vector <int> precalc_to_dia1();
    vector <int> precalc_to_dia2();
}