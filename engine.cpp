// Created by artyom-m on 3/21/19.
//
#define INIT(bitboard) bitboard = position.bitboard;
#define INIT_FIGURE(bitboard, figureIndex) figuresArray[figureIndex] = position.bitboard; \
for (int i = 0; i < 64; i++) {               \
    if ((1ull << i) & position.bitboard) {   \
        board[i] = figureIndex;              \
		mark += figureIndex ## _WEIGHT;      \
		weightByFigure[figureIndex] = figureIndex ## _WEIGHT; \
    }                                        \
}                                            \

#include "bitboard.h"
#include "engine.h"
#include "bitboard_precalc.h"
#include <list>
#include <vector>

string toHuman[64] = {"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
                      "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
                      "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
                      "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
                      "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
                      "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
                      "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
                      "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",};
int weightByFigure[600];

struct figure {
    int type;
    bitboard pos;
};

namespace Engine {
    int mark;
    move bestMove{100, 100, 100};
    int bestScore = 0;
    const ull MASK = 255;
    bitboard wFigures, bFigures, figures, figures_ver, figures_dia1, figures_dia2;
    bitboard figuresArray[1000];
    vector <int> board;

    vector<ll> visitedPositionsCnt = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int capturesCnt = 0;
    int enPassentCnt = 0;
    bool castleQ, castleK, castleq, castlek;
    bitboard enPassant;
    bitboard facticalPawn;


    void doMove(move mv, int color) {

        if (mv.from == (1ull << 63)) {
            castleK = false;
        }
        if (mv.from == (1ull << 60)) {
            castleK = false;
            castleQ = false;
        }
        if (mv.from == (1ull << 56)) {
            castleQ = false;
        }
        if (mv.from == (1ull << 0)) {
            castleq = false;
        }
        if (mv.from == (1ull << 4)) {
            castlek = false;
            castleq = false;
        }
        if (mv.from == (1ull << 7)) {
            castlek = false;
        }

        if (mv.figure == 100) {
            doMove({WHITE_KING, (1ull) << 60, (1ull) << 62}, WHITE);
            doMove({WHITE_ROOK, (1ull) << 63, (1ull) << 61}, WHITE);

            return;
        }

        if (mv.figure == 200) {
            doMove({WHITE_KING, (1ull) << 60, (1ull) << 58}, WHITE);
            doMove({WHITE_ROOK, (1ull) << 56, (1ull) << 59}, WHITE);

            return;
        }

        if (mv.figure == 300) {
            doMove({BLACK_KING, (1ull) << 4, (1ull) << 6}, BLACK);
            doMove({BLACK_ROOK, (1ull) << 7, (1ull) << 5}, BLACK);

            return;
        }

        if (mv.figure == 400) {
            doMove({BLACK_KING, (1ull) << 4, (1ull) << 2}, BLACK);
            doMove({BLACK_ROOK, (1ull) << 0, (1ull) << 3}, BLACK);

            return;
        }


        bitboard enPassantOrig = enPassant;
        bitboard facticalPawnOrig = facticalPawn;


        enPassant = 0;
        facticalPawn = 0;

        bitboard &colorFigures = (color == WHITE) ? wFigures : bFigures;
        bitboard &enemyFigures = (color == WHITE) ? bFigures : wFigures;

        int bitFrom = BitBoard::bitNumberFromBitBoard(mv.from);
        int bitTo = BitBoard::bitNumberFromBitBoard(mv.to);

        if ((mv.figure == WHITE_PAWN || mv.figure == BLACK_PAWN) && (bitFrom == bitTo + 16 || bitTo == bitFrom + 16)) {
            if (bitTo > bitFrom) {
                enPassant = 1ull << (bitFrom + 8);
            } else {
                enPassant = 1ull << (bitTo + 8);
            }
            facticalPawn = 1ull << (bitTo);
        }

        if (mv.enPassant) {
            int bitFactical = BitBoard::bitNumberFromBitBoard(facticalPawnOrig);
            int bitPassant = BitBoard::bitNumberFromBitBoard(enPassantOrig);

            int enemyPawn = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;
            figuresArray[enemyPawn] ^= (facticalPawnOrig ^ enPassantOrig);
            swap(board[bitFactical], board[bitPassant]);
            enemyFigures ^= (facticalPawnOrig ^ enPassantOrig);
            figures ^= (facticalPawnOrig ^ enPassantOrig);
            figures_ver ^= ((1ull << BitBoardPrecalc::to_ver[bitFactical]) ^ (1ull << BitBoardPrecalc::to_ver[bitPassant]));
            figures_dia1 ^= ((1ull << BitBoardPrecalc::to_dia1[bitFactical]) ^ (1ull << BitBoardPrecalc::to_dia1[bitPassant]));
            figures_dia2 ^= ((1ull << BitBoardPrecalc::to_dia2[bitFactical]) ^ (1ull << BitBoardPrecalc::to_dia2[bitPassant]));
        }

        int chopped = board[bitTo];
        board[bitTo] = board[bitFrom];
        board[bitFrom] = 0;

        figuresArray[mv.figure] ^= mv.from;
        figuresArray[mv.figure] ^= mv.to;

        if (chopped) {

            enemyFigures ^= mv.to;
            figuresArray[chopped] ^= mv.to;
            figures ^= mv.to;
            figures_ver ^= (1ull << BitBoardPrecalc::to_ver[bitTo]);
            figures_dia1 ^= (1ull << BitBoardPrecalc::to_dia1[bitTo]);
            figures_dia2 ^= (1ull << BitBoardPrecalc::to_dia2[bitTo]);

			mark -= weightByFigure[chopped];
        }

        colorFigures ^= mv.from;
        colorFigures ^= mv.to;
        figures ^= mv.from;
        figures ^= mv.to;
        figures_ver ^= (1ull << BitBoardPrecalc::to_ver[bitFrom]);
        figures_ver ^= (1ull << BitBoardPrecalc::to_ver[bitTo]);

        figures_dia1 ^= (1ull << BitBoardPrecalc::to_dia1[bitFrom]);
        figures_dia1 ^= (1ull << BitBoardPrecalc::to_dia1[bitTo]);

        figures_dia2 ^= (1ull << BitBoardPrecalc::to_dia2[bitFrom]);
        figures_dia2 ^= (1ull << BitBoardPrecalc::to_dia2[bitTo]);

        if (mv.newFigure != 0) {
            board[bitTo] = mv.newFigure;
			mark += weightByFigure[mv.newFigure];
			if (color == WHITE) {
				mark -= weightByFigure[WHITE_PAWN];
			} else {
				mark -= weightByFigure[BLACK_PAWN];
			}
            figuresArray[mv.newFigure] ^= mv.to;
            figuresArray[mv.figure] ^= mv.to;
        }


    }

    void undoMove(move mv, int color, int chopped, bitboard oldEnPassant, bitboard oldFacticalPawn) {

        if (mv.figure == 100) {
            undoMove({WHITE_KING, (1ull) << 60, (1ull) << 62}, WHITE, 0, 0, 0);
            undoMove({WHITE_ROOK, (1ull) << 63, (1ull) << 61}, WHITE, 0, 0, 0);

            return;
        }

        if (mv.figure == 200) {
            undoMove({WHITE_KING, (1ull) << 60, (1ull) << 58}, WHITE, 0, 0, 0);
            undoMove({WHITE_ROOK, (1ull) << 56, (1ull) << 59}, WHITE, 0, 0, 0);

            return;
        }

        if (mv.figure == 300) {
            undoMove({BLACK_KING, (1ull) << 4, (1ull) << 6}, BLACK, 0, 0, 0);
            undoMove({BLACK_ROOK, (1ull) << 7, (1ull) << 5}, BLACK, 0, 0, 0);

            return;
        }

        if (mv.figure == 400) {
            undoMove({BLACK_KING, (1ull) << 4, (1ull) << 2}, BLACK, 0, 0, 0);
            undoMove({BLACK_ROOK, (1ull) << 0, (1ull) << 3}, BLACK, 0, 0, 0);

            return;
        }


        bitboard &colorFigures = (color == WHITE) ? wFigures : bFigures;
        bitboard &enemyFigures = (color == WHITE) ? bFigures : wFigures;
        int bitFrom = BitBoard::bitNumberFromBitBoard(mv.from);
        int bitTo = BitBoard::bitNumberFromBitBoard(mv.to);

        if (mv.newFigure != 0) {
            board[bitTo] = mv.figure;
            figuresArray[mv.newFigure] ^= mv.to;
            figuresArray[mv.figure] ^= mv.to;
			mark -= weightByFigure[mv.newFigure];
			if (color == WHITE) {
				mark += weightByFigure[WHITE_PAWN];
			} else {
				mark += weightByFigure[BLACK_PAWN];
			}
        }

        board[bitFrom] = board[bitTo];
        board[bitTo] = chopped;

        figuresArray[mv.figure] ^= mv.from;
        figuresArray[mv.figure] ^= mv.to;
        colorFigures ^= mv.from;
        colorFigures ^= mv.to;
        figures ^= mv.from;
        figures ^= mv.to;

        if (chopped) {
            enemyFigures ^= mv.to;
            figuresArray[chopped] ^= mv.to;
            figures ^= mv.to;
            figures_ver ^= (1ull << BitBoardPrecalc::to_ver[bitTo]);
            figures_dia1 ^= (1ull << BitBoardPrecalc::to_dia1[bitTo]);
            figures_dia2 ^= (1ull << BitBoardPrecalc::to_dia2[bitTo]);

			mark += weightByFigure[chopped];
        }

        figures_ver ^= (1ull << BitBoardPrecalc::to_ver[bitFrom]);
        figures_ver ^= (1ull << BitBoardPrecalc::to_ver[bitTo]);

        figures_dia1 ^= (1ull << BitBoardPrecalc::to_dia1[bitFrom]);
        figures_dia1 ^= (1ull << BitBoardPrecalc::to_dia1[bitTo]);

        figures_dia2 ^= (1ull << BitBoardPrecalc::to_dia2[bitFrom]);
        figures_dia2 ^= (1ull << BitBoardPrecalc::to_dia2[bitTo]);

        if (mv.enPassant) {

            int enemyPawn = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;
            figuresArray[enemyPawn] ^= (oldFacticalPawn ^ oldEnPassant);

            int bitFactical = BitBoard::bitNumberFromBitBoard(oldFacticalPawn);
            int bitPassant = BitBoard::bitNumberFromBitBoard(oldEnPassant);

            swap(board[bitFactical], board[bitPassant]);
            enemyFigures ^= (oldFacticalPawn ^ oldEnPassant);
            figures ^= (oldFacticalPawn ^ oldEnPassant);

            figures_ver ^= ((1ull << BitBoardPrecalc::to_ver[bitFactical]) ^ (1ull << BitBoardPrecalc::to_ver[bitPassant]));
            figures_dia1 ^= ((1ull << BitBoardPrecalc::to_dia1[bitFactical]) ^ (1ull << BitBoardPrecalc::to_dia1[bitPassant]));
            figures_dia2 ^= ((1ull << BitBoardPrecalc::to_dia2[bitFactical]) ^ (1ull << BitBoardPrecalc::to_dia2[bitPassant]));

        }
    }

    vector<int>getBoard() {
        return board;
    }

    int getMark(int color) {
	if (color == WHITE) {
	    return mark;
	} else {
	    return -mark;
	}
    }

    void init(EPD &position) {
        mark = 0;
        board.clear();
        for (int i = 0; i < 64; i++) {
            board.push_back(0);
        }

		for (int i = 0; i < 600; i ++) {
			weightByFigure[i] = 0;
		}

        INIT_FIGURE(wKings, WHITE_KING);
        INIT_FIGURE(wQueens, WHITE_QUEEN);
        INIT_FIGURE(wRooks, WHITE_ROOK);
        INIT_FIGURE(wBishops, WHITE_BISHOP);
        INIT_FIGURE(wKnights, WHITE_KNIGHT);
        INIT_FIGURE(wPawns, WHITE_PAWN);
        INIT_FIGURE(bKings, BLACK_KING);
        INIT_FIGURE(bQueens, BLACK_QUEEN);
        INIT_FIGURE(bRooks, BLACK_ROOK);
        INIT_FIGURE(bBishops, BLACK_BISHOP);
        INIT_FIGURE(bKnights, BLACK_KNIGHT);
        INIT_FIGURE(bPawns, BLACK_PAWN);

//        INIT(color);
        INIT(castleQ);
        INIT(castleK);
        INIT(castleq);
        INIT(castlek);
//        INIT(enPassant);
        enPassant = 0;
        facticalPawn = 0;
        if (position.enPassant != "-") {
            char letter = position.enPassant[0];
            char number = position.enPassant[1];
            enPassant = (1ull) << (('8' - number) * 8 + (letter - 'a'));

            if (number == '3') {
                facticalPawn = (1ull) << (('8' - number - 1) * 8 + (letter - 'a'));
            } else {
                facticalPawn = (1ull) << (('8' - number + 1) * 8 + (letter - 'a'));
            }
        }

        wFigures = (figuresArray[WHITE_KING] | figuresArray[WHITE_QUEEN] | figuresArray[WHITE_ROOK] | figuresArray[WHITE_BISHOP] | figuresArray[WHITE_KNIGHT] | figuresArray[WHITE_PAWN]);
        bFigures = (figuresArray[BLACK_KING] | figuresArray[BLACK_QUEEN] | figuresArray[BLACK_ROOK] | figuresArray[BLACK_BISHOP] | figuresArray[BLACK_KNIGHT] | figuresArray[BLACK_PAWN]);

        figures = (bFigures | wFigures);
        figures_ver = BitBoard::conv_to_ver(figures);
        figures_dia1 = BitBoard::conv_to_dia1(figures);
        figures_dia2 = BitBoard::conv_to_dia2(figures);

        for (int i = 0; i < 15; i++) {
            visitedPositionsCnt[i] = 0ll;
        }

    };

    inline bool isCheck(int color, bitboard figurePos) {

        bitboard bbFrom = (color == BLACK) ? (figuresArray[WHITE_BISHOP] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_BISHOP] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull diagonal1 = (bitNumber & 7) + (bitNumber >> 3);
            ull mask = (figures_dia1 >> (SHIFT_DIA1[diagonal1])&(AND_DIA1[diagonal1]));
            bitboard bbTo = (BitBoardPrecalc::dia1[bitNumber][mask] & (figurePos));

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == BLACK) ? (figuresArray[WHITE_BISHOP] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_BISHOP] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull diagonal2 = (bitNumber & 7) - (bitNumber >> 3);
            ull mask = (figures_dia2 >> (SHIFT_DIA1[diagonal2 + 7])&(AND_DIA1[diagonal2 + 7]));
            bitboard bbTo = (BitBoardPrecalc::dia2[bitNumber][mask] & (figurePos));

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        bitboard figure = (color == BLACK) ? WHITE_KNIGHT : BLACK_KNIGHT;
        bbFrom = figuresArray[figure];
        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo;

            bbTo = KNIGHT_MOVES[BitBoard::bitNumberFromBitBoard(positionFrom)] & (figurePos);
            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == BLACK) ? (figuresArray[WHITE_ROOK] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_ROOK] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull horizontal = (bitNumber >> 3) << 3;
            ull mask = ((figures >> horizontal) & MASK);
            bitboard bbTo = (BitBoardPrecalc::hor[bitNumber][mask] & (figurePos));

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == BLACK) ? (figuresArray[WHITE_ROOK] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_ROOK] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull vertical = (bitNumber & 7) << 3;
            ull mask = ((figures_ver >> vertical) & MASK);
            bitboard bbTo = (BitBoardPrecalc::ver[bitNumber][mask] & (figurePos));

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }


        figure = (color == BLACK) ? WHITE_PAWN : BLACK_PAWN;
        bbFrom = figuresArray[figure];

        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo;
            if (color == BLACK) {
                bbTo = WHITE_PAWNS_CHOP[BitBoard::bitNumberFromBitBoard(positionFrom)] & (figurePos);
            } else {
                bbTo = BLACK_PAWNS_CHOP[BitBoard::bitNumberFromBitBoard(positionFrom)] & (figurePos);
            }

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        figure = (color == BLACK) ? WHITE_KING : BLACK_KING;
        bbFrom = figuresArray[figure];

        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo = KING_MOVES[BitBoard::bitNumberFromBitBoard(positionFrom)] & (figurePos);

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        return false;
    }

    void generateAgressiveMoves(vector<move>& moves, int color) {

        int figure = (color == WHITE) ? WHITE_PAWN : BLACK_PAWN;
        bitboard bbFrom = figuresArray[figure];

        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo;
            if (color == WHITE) {
                bbTo = WHITE_PAWNS_CHOP[BitBoard::bitNumberFromBitBoard(positionFrom)] & (bFigures);
            } else {
                bbTo = BLACK_PAWNS_CHOP[BitBoard::bitNumberFromBitBoard(positionFrom)] & (wFigures);
            }

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                if (color == WHITE && positionTo < (1ull << 8)) {
                    moves.push_back({figure, positionFrom, positionTo, 0, WHITE_QUEEN});
                    moves.push_back({figure, positionFrom, positionTo, 0, WHITE_ROOK});
                    moves.push_back({figure, positionFrom, positionTo, 0, WHITE_KNIGHT});
                    moves.push_back({figure, positionFrom, positionTo, 0, WHITE_BISHOP});
                } else if (color == BLACK && positionTo >= (1ull << 56)) {
                    moves.push_back({figure, positionFrom, positionTo, 0, BLACK_QUEEN});
                    moves.push_back({figure, positionFrom, positionTo, 0, BLACK_ROOK});
                    moves.push_back({figure, positionFrom, positionTo, 0, BLACK_KNIGHT});
                    moves.push_back({figure, positionFrom, positionTo, 0, BLACK_BISHOP});
                } else {
                    moves.push_back({figure, positionFrom, positionTo});
                }

                bbTo ^= positionTo;
            }

            if (enPassant) {
                bbTo = 0;
                if (color == WHITE) {
                    bbTo = WHITE_PAWNS_CHOP[BitBoard::bitNumberFromBitBoard(positionFrom)] & (enPassant);
                } else {
                    bbTo = BLACK_PAWNS_CHOP[BitBoard::bitNumberFromBitBoard(positionFrom)] & (enPassant);
                }

                if (bbTo) {
                    moves.push_back({figure, positionFrom, bbTo, true});
                }
            }

            bbFrom ^= positionFrom;
        }

        figure = (color == WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT;
        bbFrom = figuresArray[figure];
        bitboard enemyFigures = (color == WHITE) ? bFigures : wFigures;

        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo;

            bbTo = KNIGHT_MOVES[BitBoard::bitNumberFromBitBoard(positionFrom)] & (enemyFigures);


            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == WHITE) ? (figuresArray[WHITE_BISHOP] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_BISHOP] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull diagonal1 = (bitNumber & 7) + (bitNumber >> 3);
            ull mask = (figures_dia1 >> (SHIFT_DIA1[diagonal1])&(AND_DIA1[diagonal1]));
            bitboard bbTo = (BitBoardPrecalc::dia1[bitNumber][mask] & (enemyFigures));

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                int figure = WHITE_BISHOP;
                if (figuresArray[BLACK_BISHOP] & positionFrom) {
                    figure = BLACK_BISHOP;
                } else if (figuresArray[WHITE_QUEEN] & positionFrom) {
                    figure = WHITE_QUEEN;
                } else if (figuresArray[BLACK_QUEEN] & positionFrom) {
                    figure = BLACK_QUEEN;
                }
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == WHITE) ? (figuresArray[WHITE_BISHOP] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_BISHOP] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull diagonal2 = (bitNumber & 7) - (bitNumber >> 3);
            ull mask = (figures_dia2 >> (SHIFT_DIA1[diagonal2 + 7])&(AND_DIA1[diagonal2 + 7]));
            bitboard bbTo = (BitBoardPrecalc::dia2[bitNumber][mask] & (enemyFigures));

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                int figure = WHITE_BISHOP;
                if (figuresArray[BLACK_BISHOP] & positionFrom) {
                    figure = BLACK_BISHOP;
                } else if (figuresArray[WHITE_QUEEN] & positionFrom) {
                    figure = WHITE_QUEEN;
                } else if (figuresArray[BLACK_QUEEN] & positionFrom) {
                    figure = BLACK_QUEEN;
                }
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == WHITE) ? (figuresArray[WHITE_ROOK] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_ROOK] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull horizontal = (bitNumber >> 3) << 3;
            ull mask = ((figures >> horizontal) & MASK);
            bitboard bbTo = (BitBoardPrecalc::hor[bitNumber][mask] & (enemyFigures));

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                int figure = WHITE_ROOK;
                if (figuresArray[BLACK_ROOK] & positionFrom) {
                    figure = BLACK_ROOK;
                } else if (figuresArray[WHITE_QUEEN] & positionFrom) {
                    figure = WHITE_QUEEN;
                } else if (figuresArray[BLACK_QUEEN] & positionFrom) {
                    figure = BLACK_QUEEN;
                }
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == WHITE) ? (figuresArray[WHITE_ROOK] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_ROOK] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull vertical = (bitNumber & 7) << 3;
            ull mask = ((figures_ver >> vertical) & MASK);
            bitboard bbTo = (BitBoardPrecalc::ver[bitNumber][mask] & (enemyFigures));

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                int figure = WHITE_ROOK;
                if (figuresArray[BLACK_ROOK] & positionFrom) {
                    figure = BLACK_ROOK;
                } else if (figuresArray[WHITE_QUEEN] & positionFrom) {
                    figure = WHITE_QUEEN;
                } else if (figuresArray[BLACK_QUEEN] & positionFrom) {
                    figure = BLACK_QUEEN;
                }
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

        figure = (color == WHITE) ? WHITE_KING : BLACK_KING;
        bbFrom = figuresArray[figure];

        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo = KING_MOVES[BitBoard::bitNumberFromBitBoard(positionFrom)] & (enemyFigures);

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }


    }

    inline void generateSilentMoves(vector<move> &moves, int color) {

        if (castleK && color == WHITE && board[60] == WHITE_KING && board[63] == WHITE_ROOK && board[61] == 0 && board[62] == 0 && !isCheck(WHITE, (1ull << 60)) && !isCheck(WHITE, (1ull << 61))) {
            moves.push_back({100, 100, 100});
        }

        if (castleQ && color == WHITE && board[60] == WHITE_KING && board[56] == WHITE_ROOK && board[57] == 0 && board[59] == 0 && board[58] == 0 && !isCheck(WHITE, (1ull << 60)) && !isCheck(WHITE, (1ull << 59))) {
            moves.push_back({200, 200, 200});
        }

        if (castlek && color == BLACK && board[4] == BLACK_KING && board[7] == BLACK_ROOK && board[5] == 0 && board[6] == 0 && !isCheck(BLACK, (1ull << 4)) && !isCheck(BLACK, (1ull << 5))) {
            moves.push_back({300, 300, 300});
        }


        if (castleq && color == BLACK && board[4] == BLACK_KING && board[0] == BLACK_ROOK && board[1] == 0 && board[2] == 0 && board[3] == 0 && !isCheck(BLACK, (1ull << 4)) && !isCheck(BLACK, (1ull << 3))) {
            moves.push_back({400, 400, 400});
        }

        int figure = (color == WHITE) ? WHITE_KING : BLACK_KING;
        bitboard bbFrom = figuresArray[figure];

        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo = KING_MOVES[BitBoard::bitNumberFromBitBoard(positionFrom)] & (~figures);

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                moves.push_back({figure, positionFrom, positionTo});


                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

        figure = (color == WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT;
        bbFrom = figuresArray[figure];

        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo = KNIGHT_MOVES[BitBoard::bitNumberFromBitBoard(positionFrom)] & (~figures);

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }
        figure = (color == WHITE ? WHITE_PAWN : BLACK_PAWN);
        bbFrom = figuresArray[figure];
        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            bitboard bbTo = (color == WHITE) ? WHITE_PAWNS_SHORT_MOVES[bitNumber] & (~figures)
                                             : BLACK_PAWNS_SHORT_MOVES[bitNumber] & (~figures);
            int numberRow = (bitNumber >> 3);

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                if (color == WHITE && positionTo < (1ull << 8)) {
                    moves.push_back({figure, positionFrom, positionTo, 0, WHITE_QUEEN});
                    moves.push_back({figure, positionFrom, positionTo, 0, WHITE_ROOK});
                    moves.push_back({figure, positionFrom, positionTo, 0, WHITE_KNIGHT});
                    moves.push_back({figure, positionFrom, positionTo, 0, WHITE_BISHOP});
                } else if (color == BLACK && positionTo >= (1ull << 56)) {
                    moves.push_back({figure, positionFrom, positionTo, 0, BLACK_QUEEN});
                    moves.push_back({figure, positionFrom, positionTo, 0, BLACK_ROOK});
                    moves.push_back({figure, positionFrom, positionTo, 0, BLACK_KNIGHT});
                    moves.push_back({figure, positionFrom, positionTo, 0, BLACK_BISHOP});
                } else {
                    moves.push_back({figure, positionFrom, positionTo});
                }
                if (numberRow == 1 && color == BLACK || numberRow == 6 && color == WHITE) {
                    bitboard positionTo2 = (color == WHITE) ? WHITE_PAWNS_LONG_MOVES[bitNumber] & (~figures) : BLACK_PAWNS_LONG_MOVES[bitNumber] & (~figures);
                    if (positionTo2) {
                        moves.push_back({figure, positionFrom, positionTo2});
                    }
                }
                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == WHITE) ? (figuresArray[WHITE_ROOK] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_ROOK] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull horizontal = (bitNumber >> 3) << 3;
            ull mask = ((figures >> horizontal) & MASK);
            bitboard bbTo = (BitBoardPrecalc::hor[bitNumber][mask] & (~figures));

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                int figure = WHITE_ROOK;
                if (figuresArray[BLACK_ROOK] & positionFrom) {
                    figure = BLACK_ROOK;
                } else if (figuresArray[WHITE_QUEEN] & positionFrom) {
                    figure = WHITE_QUEEN;
                } else if (figuresArray[BLACK_QUEEN] & positionFrom) {
                    figure = BLACK_QUEEN;
                }
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == WHITE) ? (figuresArray[WHITE_ROOK] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_ROOK] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull vertical = (bitNumber & 7) << 3;
            ull mask = ((figures_ver >> vertical) & MASK);
            bitboard bbTo = (BitBoardPrecalc::ver[bitNumber][mask] & (~figures));

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                int figure = WHITE_ROOK;
                if (figuresArray[BLACK_ROOK] & positionFrom) {
                    figure = BLACK_ROOK;
                } else if (figuresArray[WHITE_QUEEN] & positionFrom) {
                    figure = WHITE_QUEEN;
                } else if (figuresArray[BLACK_QUEEN] & positionFrom) {
                    figure = BLACK_QUEEN;
                }
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == WHITE) ? (figuresArray[WHITE_BISHOP] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_BISHOP] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull diagonal1 = (bitNumber & 7) + (bitNumber >> 3);
            ull mask = (figures_dia1 >> (SHIFT_DIA1[diagonal1])&(AND_DIA1[diagonal1]));
            bitboard bbTo = (BitBoardPrecalc::dia1[bitNumber][mask] & (~figures));

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                int figure = WHITE_BISHOP;
                if (figuresArray[BLACK_BISHOP] & positionFrom) {
                    figure = BLACK_BISHOP;
                } else if (figuresArray[WHITE_QUEEN] & positionFrom) {
                    figure = WHITE_QUEEN;
                } else if (figuresArray[BLACK_QUEEN] & positionFrom) {
                    figure = BLACK_QUEEN;
                }
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == WHITE) ? (figuresArray[WHITE_BISHOP] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_BISHOP] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull diagonal2 = (bitNumber & 7) - (bitNumber >> 3);
            ull mask = (figures_dia2 >> (SHIFT_DIA1[diagonal2 + 7])&(AND_DIA1[diagonal2 + 7]));
            bitboard bbTo = (BitBoardPrecalc::dia2[bitNumber][mask] & (~figures));

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                int figure = WHITE_BISHOP;
                if (figuresArray[BLACK_BISHOP] & positionFrom) {
                    figure = BLACK_BISHOP;
                } else if (figuresArray[WHITE_QUEEN] & positionFrom) {
                    figure = WHITE_QUEEN;
                } else if (figuresArray[BLACK_QUEEN] & positionFrom) {
                    figure = BLACK_QUEEN;
                }
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

    }

    int alphabeta(int color, int ply, int depth, int alpha, int beta) {
        visitedPositionsCnt[depth] ++;

        if (depth == 0) {
            return getMark(color) + rand() % 10;
        }
        vector<move> moves;
        vector<bool> notCheck;
        generateAgressiveMoves(moves, color);
        generateSilentMoves(moves, color);

		int good_moves = 0;

        for (auto mv: moves) {

			if (alpha >= beta) {
				return alpha;
			}
	
            int chopped = board[BitBoard::bitNumberFromBitBoard(mv.to)];
            bitboard oldEnPassant = enPassant;
            bitboard oldFacticalPawn = facticalPawn;
            bool oldCastleK = castleK;
            bool oldCastleQ = castleQ;
            bool oldCastlek = castlek;
            bool oldCastleq = castleq;
            doMove(mv, color);

            bitboard kingPos = (color == WHITE) ? figuresArray[WHITE_KING] : figuresArray[BLACK_KING];
            if (!isCheck(color, kingPos)) {
				good_moves ++;
                notCheck.push_back(true);
                int subMark = -alphabeta(color ^ WHITE_BLACK, ply + 1, depth - 1, -beta, -alpha);
				if (subMark > alpha) {
					alpha = subMark;
					if (ply == 0) {
						bestMove = mv;
						bestScore = subMark;
					}
				}
/*		if (ply == 0) {
			cout << "move " << toHuman[BitBoard::bitNumberFromBitBoard(mv.from)]
			     << toHuman[BitBoard::bitNumberFromBitBoard(mv.to)] << " " << subMark << endl;
		}*/

            } else {
                notCheck.push_back(false);
            }
            if (mv.enPassant) {
                chopped = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;
            }
            undoMove(mv, color, chopped, oldEnPassant, oldFacticalPawn);
            enPassant = oldEnPassant;
            facticalPawn = oldFacticalPawn;
            castleK = oldCastleK;
            castleQ = oldCastleQ;
            castlek = oldCastlek;
            castleq = oldCastleq;

        }

		if (good_moves == 0) {
				bitboard kingPos = (color == WHITE) ? figuresArray[WHITE_KING] : figuresArray[BLACK_KING];
				if (isCheck(color, kingPos)) {
					return -oo + ply * 1000;
				} else {
					return 0;
				}
		}

        if (mode == engineMode::Game || mode == engineMode::Launch) {
            if (ply == 0) {
		cout << "info depth 123 score cp " << bestScore / 10 << " time 0 nodes 0 pv e2e4\n";
                if (bestMove.figure == 100) {
                    cout << "bestmove e1g1\n";
                } else if (bestMove.figure == 200) {
                    cout << "bestmove e1c1\n";
                } else if (bestMove.figure == 300) {
                    cout << "bestmove e8g8\n";
                } else if (bestMove.figure == 400) {
                    cout << "bestmove e8c8\n";
                } else {
                    if (!bestMove.newFigure) {
                        cout << "bestmove " << toHuman[BitBoard::bitNumberFromBitBoard(bestMove.from)]
                             << toHuman[BitBoard::bitNumberFromBitBoard(bestMove.to)] << "\n";
                    } else {
                        cout << "bestmove " << toHuman[BitBoard::bitNumberFromBitBoard(bestMove.from)]
                             << toHuman[BitBoard::bitNumberFromBitBoard(bestMove.to)];
                        if (bestMove.newFigure & QUEEN) {
                            cout << "q\n";
                        }
                        if (bestMove.newFigure & BISHOP) {
                            cout << "b\n";
                        }
                        if (bestMove.newFigure & ROOK) {
                            cout << "b\n";
                        }
                        if (bestMove.newFigure & KNIGHT) {
                            cout << "n\n";
                        }
                    }
                }
            }
        }

	return alpha;


    }


    vector<ll> getVisitedPositionsCnt() {
       return visitedPositionsCnt;
    }

    void printBoard() {
        string s = "abcdefgh";
        cout << "  ";
        for (int i = 0; i < 8; i ++) {
            cout << s[i] << " ";
        }
        cout << "\n";
        for (int i = 0; i < 64; i++) {

            if (i % 8 == 0) {
                cout << 8 - (i/8);
            }

            cout << "|";
            if (board[i] == WHITE_KING) {
                cout << "♔";
            } else if (board[i] == WHITE_QUEEN) {
                cout << "♕";
            } else if (board[i] == WHITE_ROOK) {
                cout << "♖";
            } else if (board[i] == WHITE_BISHOP) {
                cout << "♗";
            } else if (board[i] == WHITE_KNIGHT) {
                cout << "♘";
            } else if (board[i] == WHITE_PAWN) {
                cout << "♙";
            } else if (board[i] == BLACK_KING) {
                cout << "♚";
            } else if (board[i] == BLACK_QUEEN) {
                cout << "♛";
            } else if (board[i] == BLACK_ROOK) {
                cout << "♜";
            } else if (board[i] == BLACK_BISHOP) {
                cout << "♝";
            } else if (board[i] == BLACK_KNIGHT) {
                cout << "♞";
            } else if (board[i] == BLACK_PAWN) {
                cout << "♟";
            } else {
                cout << " ";
            }

            if (i % 8 == 7) {
                cout << "|" << 8 - (i/8) << "\n";
            }
        }
        cout << " ";
        for (int i = 0; i < 8; i ++) {
            cout << s[i] << " ";
        }
        cout << "\n";
    }

}
