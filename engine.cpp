//
// Created by artyom-m on 3/21/19.
//
#define INIT(bitboard) bitboard = position.bitboard;
#define INIT_FIGURE(bitboard, figureIndex) figuresArray[figureIndex] = position.bitboard;

#define FIGURE_CASE(figure, colorFigures) if (figure & mv.from) { \
figure ^= mv.from;                                      \
figure ^= mv.to;                                        \
colorFigures ^= mv.from;                                \
colorFigures ^= mv.to;                                  \
figures ^= mv.from;                                     \
figures ^= mv.to;                                       \
}

#define UNDO_FIGURE_CASE(figure, colorFigures) if (figure & mv.to) { \
figure ^= mv.from;                                      \
figure ^= mv.to;                                        \
colorFigures ^= mv.from;                                \
colorFigures ^= mv.to;                                  \
figures ^= mv.from;                                     \
figures ^= mv.to;                                       \
}\

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
//    bitboard wKings, wQueens, wRooks, wBishops, wKnights, wPawns;
//    bitboard bKings, bQueens, bRooks, bBishops, bKnights, bPawns;
    bitboard wFigures, bFigures, figures;
    bitboard figuresArray[10];

//    int color;
    vector<ll> visitedPositionsCnt = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    bool castleQ, castleK, castleq, castlek;
    string enPassant;

    std::list<figure> figuresList;

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
        if (figuresArray[mv.figure] & mv.from) {
            figuresArray[mv.figure] ^= mv.from;
            figuresArray[mv.figure] ^= mv.to;
            colorFigures ^= mv.from;
            colorFigures ^= mv.to;
            figures ^= mv.from;
            figures ^= mv.to;
        }

    }

    inline void undoMove(move mv, int color) {
        bitboard &colorFigures = (color == WHITE) ? wFigures : bFigures;
        if (figuresArray[mv.figure] & mv.to) {
            figuresArray[mv.figure] ^= mv.from;
            figuresArray[mv.figure] ^= mv.to;
            colorFigures ^= mv.from;
            colorFigures ^= mv.to;
            figures ^= mv.from;
            figures ^= mv.to;
        }
    }

    void init(EPD &position) {
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

        for (int i = 0; i < 15; i++) {
            visitedPositionsCnt[i] = 0ll;
        }
    };

    inline list<move> generateMoves(int color) {
        list<move> moves;

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
/*
        int frbb = frombb(p);
        ull h = (frbb >> 3) << 3;
        ull msk = ((figures >> h) & MASK);
        ull x2 = (hor[frbb][msk]&(~figures));*/

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


        return moves;

    }

    void alphabeta(int color, int depth, int alpha, int beta) {
        visitedPositionsCnt[depth] ++;
        if (depth == 0) {
            return;
        }
        list<move> moves = generateMoves(color);

        for (auto mv: moves) {

            doMove(mv, color);

            alphabeta(color ^ WHITE_BLACK, depth - 1, -beta, -alpha);

            undoMove(mv, color);
        }

    }

    vector<ll> getVisitedPositionsCnt() {
       return visitedPositionsCnt;
    }

}