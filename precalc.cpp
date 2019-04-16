//
// Created by artyom-m on 4/16/19.
//

#include "bitboard.h"
#include "precalc.h"

namespace Precalc {

    vector <vector <bitboard> > precalc_hor() {
        vector <vector <bitboard> > result;
        for (ull i = 0; i < 64; i++) {
            result.push_back({});
            bitboard bb = 0;

            int x1 = i / 8;
            int x2 = i % 8;

            for (ull j = 0; j < 256; j++) {

                bb = 0;

                for (int k = x2 - 1; k >= 0; k--) {

                    bb |= (1ull << (8 * x1 + k));
                    if (j & (1ull << k)) break;
                }

                for (ull k = x2 + 1; k < 8; k++) {

                    bb |= (1ull << (8 * x1 + k));
                    if (j & (1ull << k)) break;
                }

                result[i].push_back(bb);
            }

        }

        return result;

    }
    vector <vector <bitboard> > precalc_ver() {

        vector <vector <bitboard> > result;

        for (ull i = 0; i < 64; i++) {
            result.push_back({});

            int x1 = i / 8;
            int x2 = i % 8;

            for (ull j = 0; j < 256; j++) {

                bitboard bb = 0;

                for (int k = x1 - 1; k >= 0; k--) {

                    bb |= (1ull << (8 * k + x2));
                    if (j & (1ull << k)) break;
                }

                for (ull k = x1 + 1; k < 8; k++) {

                    bb |= (1ull << (8 * k + x2));
                    if (j & (1ull << k)) break;
                }

                result[i].push_back(bb);

            }

        }

        return result;
    }

    vector <int> precalc_to_ver() {
        vector <int> result;
        for (int i = 0; i < 64; i++) {
            int x1 = i / 8;
            int x2 = i % 8;
            result.push_back(x1 + x2 * 8);

        }
        return result;
    }
}