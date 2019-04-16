//
// Created by artyom-m on 4/16/19.
//

#include "bitboard.h"
#include "precalc.h"

#include <cmath>

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

    vector <vector <bitboard> > precalc_dia1() {
        vector <vector <bitboard> > result;
        for (int i = 0; i < 64; i++) {
            result.push_back({});

            int x1 = i / 8;
            int x2 = i % 8;

            int cnt = 0;
            int num = 0;
            int all = min(x1 + x2, 7) - max(0, x1 + x2 - 7);
            for (int k = max(0, x1 + x2 - 7); k <= min(x1 + x2, 7); k++) {
                if (x1 == k) num = cnt;
                cnt++;
            }

            for (int j = 0; j < 256; j++) {

                int x3 = x1;
                int x4 = x2;
                int num2 = num;

                ull x = 0;

                while (true) {
                    x3--;
                    x4++;
                    num2--;
                    if (num2 < 0) break;
                    x += (1ull << (x3 * 8 + x4));
                    if (((1ull << num2) & j) > 0) break;
                }

                num2 = num;
                x3 = x1;
                x4 = x2;

                while (true) {
                    x3++;
                    x4--;
                    num2++;
                    if (num2 > all) break;
                    x += (1ull << (x3 * 8 + x4));
                    if (((1ull << num2) & j) > 0) break;
                }

                result[i].push_back({x});
            }

        }

        return result;
    }

    vector <vector <bitboard> > precalc_dia2() {
        vector <vector <bitboard> > result;
        for (int i = 0; i < 64; i++) {
            result.push_back({});

            int x1 = i / 8;
            int x2 = i % 8;

            int num = min(x1, x2);
            int all = 7 - abs(x1 - x2);

            for (int j = 0; j < 256; j++) {

                int x3 = x1;
                int x4 = x2;
                int num2 = num;

                ull x = 0;

                while (true) {
                    x3--;
                    x4--;
                    num2--;
                    if (num2 < 0) break;
                    x += (1ull << (x3 * 8 + x4));
                    if (((1ull << num2) & j) > 0) break;
                }

                num2 = num;
                x3 = x1;
                x4 = x2;

                while (true) {
                    x3++;
                    x4++;
                    num2++;
                    if (num2 > all) break;
                    x += (1ull << (x3 * 8 + x4));
                    if (((1ull << num2) & j) > 0) break;
                }
                result[i].push_back({x});
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

    vector <int> precalc_to_dia1() {
        vector <int> result(64, 0);
        int cnt = 0;
        for (int i = 0; i <= 14; i++) {
            for (int j = 0; j <= 7; j++)
                for (int k = 0; k <= 7; k++)
                    if (j + k == i) {

                        result[j * 8 + k] = cnt;
                        cnt++;
                    }

        }
        return result;
    }

    vector <int> precalc_to_dia2() {
        vector <int> result(64, 0);
        int cnt = 0;
        for (int i = -7; i <= 7; i++) {
            for (int j = 0; j <= 7; j++)
                for (int k = 0; k <= 7; k++)
                    if (k - j == i) {
                        result[j * 8 + k] = cnt;
                        cnt++;
                    }
        }
        return result;
    }
}