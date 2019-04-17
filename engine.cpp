//
// Created by artyom-m on 3/21/19.
//
#define INIT(bitboard) bitboard = position.bitboard;
#define INIT_FIGURE(bitboard, figureIndex) figuresArray[figureIndex] = position.bitboard;\
for (int i = 0; i < 64; i++) {               \
    if ((1ull << i) & position.bitboard) {   \
        board[i] = figureIndex;              \
    }                                        \
}                                            \

#include "bitboard.h"
#include "engine.h"
#include "bitboard_precalc.h"
#include <list>
#include <vector>


struct figure {
    int type;
    bitboard pos;
};

namespace Engine {
    const ull MASK = 255;
    bitboard wFigures, bFigures, figures, figures_ver, figures_dia1, figures_dia2;
    bitboard figuresArray[1000];
    int board[64];

    vector<ll> visitedPositionsCnt = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int capturesCnt = 0;
    bool castleQ, castleK, castleq, castlek;
    string enPassant;

    struct move {
        int figure;
        bitboard from;
        bitboard to;

        move(int Figure, bitboard From, bitboard To) :
                figure(Figure),
                from(From),
                to(To) {
        }
    };

    inline void doMove(move mv, int color) {
        bitboard &colorFigures = (color == WHITE) ? wFigures : bFigures;
        bitboard &enemyFigures = (color == WHITE) ? bFigures : wFigures;

        int bitFrom = BitBoard::bitNumberFromBitBoard(mv.from);
        int bitTo = BitBoard::bitNumberFromBitBoard(mv.to);

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


    }

    inline void undoMove(move mv, int color, int chopped) {
        bitboard &colorFigures = (color == WHITE) ? wFigures : bFigures;
        bitboard &enemyFigures = (color == WHITE) ? bFigures : wFigures;
        int bitFrom = BitBoard::bitNumberFromBitBoard(mv.from);
        int bitTo = BitBoard::bitNumberFromBitBoard(mv.to);

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
        }

        figures_ver ^= (1ull << BitBoardPrecalc::to_ver[bitFrom]);
        figures_ver ^= (1ull << BitBoardPrecalc::to_ver[bitTo]);

        figures_dia1 ^= (1ull << BitBoardPrecalc::to_dia1[bitFrom]);
        figures_dia1 ^= (1ull << BitBoardPrecalc::to_dia1[bitTo]);

        figures_dia2 ^= (1ull << BitBoardPrecalc::to_dia2[bitFrom]);
        figures_dia2 ^= (1ull << BitBoardPrecalc::to_dia2[bitTo]);

    }

    void init(EPD &position) {

        for (int i = 0; i < 64; i++) {
            board[i] = 0;
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
        INIT(enPassant);

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

    inline bool isCheck(int color) {
        bitboard kingPos = (color == WHITE) ? figuresArray[WHITE_KING] : figuresArray[BLACK_KING];
        bitboard bbFrom = (color == BLACK) ? (figuresArray[WHITE_BISHOP] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_BISHOP] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull diagonal1 = (bitNumber & 7) + (bitNumber >> 3);
            ull mask = (figures_dia1 >> (SHIFT_DIA1[diagonal1])&(AND_DIA1[diagonal1]));
            bitboard bbTo = (BitBoardPrecalc::dia1[bitNumber][mask] & (kingPos));

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == BLACK) ? (figuresArray[WHITE_BISHOP] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_BISHOP] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull diagonal2 = (bitNumber & 7) - (bitNumber >> 3);
            ull mask = (figures_dia2 >> (SHIFT_DIA1[diagonal2 + 7])&(AND_DIA1[diagonal2 + 7]));
            bitboard bbTo = (BitBoardPrecalc::dia2[bitNumber][mask] & (kingPos));

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        bitboard figure = (color == BLACK) ? WHITE_KNIGHT : BLACK_KNIGHT;
        bbFrom = figuresArray[figure];
        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo;

            bbTo = KNIGHT_MOVES[BitBoard::bitNumberFromBitBoard(positionFrom)] & (kingPos);
            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == BLACK) ? (figuresArray[WHITE_ROOK] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_ROOK] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull horizontal = (bitNumber >> 3) << 3;
            ull mask = ((figures >> horizontal) & MASK);
            bitboard bbTo = (BitBoardPrecalc::hor[bitNumber][mask] & (kingPos));

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        bbFrom = (color == BLACK) ? (figuresArray[WHITE_ROOK] | figuresArray[WHITE_QUEEN]) : (figuresArray[BLACK_ROOK] | figuresArray[BLACK_QUEEN]);
        while (bbFrom) {

            bitboard positionFrom = (bbFrom & -bbFrom);
            int bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            ull vertical = (bitNumber & 7) << 3;
            ull mask = ((figures_ver >> vertical) & MASK);
            bitboard bbTo = (BitBoardPrecalc::ver[bitNumber][mask] & (kingPos));

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }


        figure = (color == BLACK) ? WHITE_PAWN : BLACK_PAWN;
        bbFrom = figuresArray[figure];

        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo;
            if (color == BLACK) {
                bbTo = WHITE_PAWNS_CHOP[BitBoard::bitNumberFromBitBoard(positionFrom)] & (kingPos);
            } else {
                bbTo = BLACK_PAWNS_CHOP[BitBoard::bitNumberFromBitBoard(positionFrom)] & (kingPos);
            }

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        figure = (color == BLACK) ? WHITE_KING : BLACK_KING;
        bbFrom = figuresArray[figure];

        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo = KING_MOVES[BitBoard::bitNumberFromBitBoard(positionFrom)] & (kingPos);

            if (bbTo) return true;

            bbFrom ^= positionFrom;
        }

        return false;
    }

    void generateAgressiveMoves(list<move>& moves, int color) {

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
                moves.push_back({figure, positionFrom, positionTo});

                bbTo ^= positionTo;
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

    inline void generateSilentMoves(list<move> &moves, int color) {

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
                moves.push_back({figure, positionFrom, positionTo});
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

    void alphabeta(int color, int depth, int alpha, int beta) {
        visitedPositionsCnt[depth] ++;
        if (depth == 0) {
            return;
        }
        list<move> moves;
        generateAgressiveMoves(moves, color);
        generateSilentMoves(moves, color);

        for (auto mv: moves) {

            int chopped = board[BitBoard::bitNumberFromBitBoard(mv.to)];

            doMove(mv, color);
            if (!isCheck(color)) {
                alphabeta(color ^ WHITE_BLACK, depth - 1, -beta, -alpha);
            }
            undoMove(mv, color, chopped);


        }

    }

    vector<ll> getVisitedPositionsCnt() {
       return visitedPositionsCnt;
    }

}