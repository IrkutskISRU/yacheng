//
// Created by artyom-m on 4/15/19.
//

#include "constants.h"
#include <iostream>
#include <vector>

namespace Constants {

    void init_knights() {

        for (int i = 0; i < 64; i++) {
            bitboard result = 0;

            int x = i % 8;
            int y = i / 8;

            std::vector <std::pair <int, int> > directions = {{-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {-2, -1}, {-2, 1}, {2, -1}, {2, 1}};

            for (auto direction: directions) {
                if (direction.first + x >= 0 && direction.first + x <= 7 &&
                    direction.second + y >= 0 && direction.second + y <= 7) {
                    result += (1ull << ((direction.first + x) + (direction.second + y) * 8));
                }
            }
            std::cout << result << ", ";
            if (i%8 == 7) {
                std::cout << "\n";
            }
//            BitBoard::print(result);
//            std::cout << "\n";
        }

    }

    void init_queens() {

        for (int i = 0; i < 64; i++) {
            bitboard result = 0;
            if (i % 8 != 0) {
                result += 1ull << (i - 1);
            }
            if (i % 8 != 7) {
                result += 1ull << (i + 1);
            }
            if (i / 8 != 0) {
                result += 1ull << (i - 8);
            }
            if (i / 8 != 7) {
                result += 1ull << (i + 8);
            }

            if (i % 8 != 0 && i / 8 != 0) {
                result += 1ull << (i - 9);
            }
            if (i % 8 != 0 && i / 8 != 7) {
                result += 1ull << (i + 7);
            }
            if (i % 8 != 7 && i / 8 != 0) {
                result += 1ull << (i - 7);
            }
            if (i % 8 != 7 && i / 8 != 7) {
                result += 1ull << (i + 9);
            }

            std::cout << result << ", ";
            if (i%8 == 7) {
                std::cout << "\n";
            }

        }

    }

    void init_pawns() {
        std::cout << "BLACK SHORT\n";
        for (int i = 0; i < 64; i++) {
            bitboard result = 0;

            if (i / 8 != 7) {
                result += (1ull << (i + 8));
            }
            std::cout << result << ", ";
            if (i%8 == 7) std::cout << "\n";

        }
        std::cout << "BLACK LONG\n";
        for (int i = 0; i < 64; i++) {
            bitboard result = 0;

            if (i / 8 == 1) {
                result += (1ull << (i + 16));
            }
            std::cout << result << ", ";
            if (i%8 == 7) std::cout << "\n";

        }
        std::cout << "WHITE SHORT\n";
        for (int i = 0; i < 64; i++) {
            bitboard result = 0;

            if (i / 8 != 0) {
                result += (1ull << (i - 8));
            }

//            BitBoard::print(result);
//            std::cout << "\n";
            std::cout << result << ", ";
            if (i%8 == 7) std::cout << "\n";
        }
        std::cout << "WHITE LONG\n";
        for (int i = 0; i < 64; i++) {
            bitboard result = 0;

            if (i / 8 == 6) {
                result += (1ull << (i - 16));
            }
            std::cout << result << ", ";
            if (i%8 == 7) std::cout << "\n";

        }
    }

    void init() {
    }
}
