//
// Created by artyom-m on 3/21/19.
//

#include "board.h"

EPD::EPD(const string& s) {

    reset();

    int numberOfBit = 0;
    for (auto ch: s) {

        if (numberOfBit < 64) {
            switch (ch) {
                case 'K': {
                    wKings ^= (1ull << numberOfBit);
                }
                case 'Q': {
                    wQueens ^= (1ull << numberOfBit);
                }
                case 'R': {
                    wRooks ^= (1ull << numberOfBit);
                }
                case 'B': {
                    wBishops ^= (1ull << numberOfBit);
                }
                case 'N': {
                    wKnights ^= (1ull << numberOfBit);
                }
                case 'P': {
                    wPawns ^= (1ull << numberOfBit);
                }
                case 'k': {
                    bKings ^= (1ull << numberOfBit);
                }
                case 'q': {
                    bQueens ^= (1ull << numberOfBit);
                }
                case 'r': {
                    bRooks ^= (1ull << numberOfBit);
                }
                case 'b': {
                    bBishops ^= (1ull << numberOfBit);
                }
                case 'n': {
                    bKnights ^= (1ull << numberOfBit);
                }
                case 'p': {
                    bPawns ^= (1ull << numberOfBit);
                }
            }
        }


        if (ch >= '0' && ch <= '8') {
            numberOfBit += ch - '0';
        } else if (ch != '/') {
            numberOfBit++;
        }

        if (numberOfBit == 66) {
            if (ch == 'w') {
                color = WHITE;
            } else {
                color = BLACK;
            }
        }

        if (numberOfBit > 66) {
            if (ch == 'K') castleK = true;
            if (ch == 'Q') castleQ = true;
            if (ch == 'k') castlek = true;
            if (ch == 'q') castleq = true;
        }

    }

    if (s[s.length()-1] != '-') {
        enPassant = s.substr(s.length()-2, 2);
    } else {
        enPassant = "-";
    }

}

void EPD::print() const {
    string s = "abcdefgh";
    cout << "  ";
    for (int i = 0; i < 8; i ++)
        cout << s[i] << " ";
    cout << "\n";

    for (int i = 0; i < 8; i++) {
        cout << 8-i;
        for (int j = 0; j < 8; j++) {
            cout << "|";

            int numberOfBit = i * 8 + j;

            unsigned long long bitValue = (1ull << numberOfBit);

            if (wKings & bitValue) {
                cout << "♔";
            } else if (wQueens & bitValue) {
                cout << "♕";
            } else if (wRooks & bitValue) {
                cout << "♖";
            } else if (wBishops & bitValue) {
                cout << "♗";
            } else if (wKnights & bitValue) {
                cout << "♘";
            } else if (wPawns & bitValue) {
                cout << "♙";
            } else if (bKings & bitValue) {
                cout << "♚";
            } else if (bQueens & bitValue) {
                cout << "♛";
            } else if (bRooks & bitValue) {
                cout << "♜";
            } else if (bBishops & bitValue) {
                cout << "♝";
            } else if (bKnights & bitValue) {
                cout << "♞";
            } else if (bPawns & bitValue) {
                cout << "♟";
            } else {
                cout << " ";
            }
        }
        cout << "|" << 8-i << "\n";
    }

    cout << "  ";
    for (int i = 0; i < 8; i ++)
        cout << s[i] << " ";
    cout << "\n";

    cout << (color == WHITE ? "WHITE MOVE" : "BLACK MOVE") << "\n";
    cout << "Castles: " << castleQ << " " << castleK << " " << castleq << " " << castlek << "\n";
    cout << "En passant " << enPassant << "\n";
}

void EPD::reset() {
    wKings = 0;
    wQueens = 0;
    wRooks = 0;
    wBishops = 0;
    wKnights = 0;
    wPawns = 0;
    bKings = 0;
    bQueens = 0;
    bRooks = 0;
    bBishops = 0;
    bKnights = 0;
    bPawns = 0;

    castleK = false;
    castleQ = false;
    castleq = false;
    castlek = false;

    enPassant = "";
}