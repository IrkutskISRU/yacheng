//
// Created by artyom-m on 3/21/19.
//

#define INIT(variable) variable = position.variable;

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
#include <list>
#include <vector>

struct figure {
    int type;
    bitboard pos;
};

namespace Engine {
    bitboard wKings, wQueens, wRooks, wBishops, wKnights, wPawns;
    bitboard bKings, bQueens, bRooks, bBishops, bKnights, bPawns;
    bitboard wFigures, bFigures, figures;

//    int color;
    vector<ll> visitedPositionsCnt = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    bool castleQ, castleK, castleq, castlek;
    string enPassant;

    std::list<figure> figuresList;

    struct move {
        bitboard from;
        bitboard to;

        move(bitboard From, bitboard To) :
                from(From),
                to(To) {
        }
    };

    inline void doMove(move mv) {

        FIGURE_CASE(wKings, wFigures)
        FIGURE_CASE(wKnights, wFigures)
        FIGURE_CASE(wPawns, wFigures)
        FIGURE_CASE(bKings, bFigures)
        FIGURE_CASE(bKnights, bFigures)
        FIGURE_CASE(bPawns, bFigures)
    }

    inline void undoMove(move mv) {
        UNDO_FIGURE_CASE(wKings, wFigures)
        UNDO_FIGURE_CASE(wKnights, wFigures)
        UNDO_FIGURE_CASE(wPawns, wFigures)
        UNDO_FIGURE_CASE(bKings, bFigures)
        UNDO_FIGURE_CASE(bKnights, bFigures)
        UNDO_FIGURE_CASE(bPawns, bFigures)
    }

    void init(EPD &position) {
        INIT(wKings);
        INIT(wQueens);
        INIT(wRooks);
        INIT(wBishops);
        INIT(wKnights);
        INIT(wPawns);
        INIT(bKings);
        INIT(bQueens);
        INIT(bRooks);
        INIT(bBishops);
        INIT(bKnights);
        INIT(bPawns);
//        INIT(color);
        INIT(castleQ);
        INIT(castleK);
        INIT(castleq);
        INIT(castlek);
        INIT(enPassant);

        wFigures = (wKings | wQueens | wRooks | wBishops | wKnights | wPawns);
        bFigures = (bKings | bQueens | bRooks | bBishops | bKnights | bPawns);
        figures = (bFigures | wFigures);

        for (int i = 0; i < 15; i++) {
            visitedPositionsCnt[i] = 0ll;
        }
    };

    inline list<move> generateMoves(int color) {
        list<move> moves;


        bitboard bbFrom = (color == WHITE) ? wKings : bKings;
        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo = KING_MOVES[BitBoard::bitNumberFromBitBoard(positionFrom)] & (~figures);

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                moves.push_back({positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }
        bbFrom = (color == WHITE) ? wKnights : bKnights;
        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bbTo = KNIGHT_MOVES[BitBoard::bitNumberFromBitBoard(positionFrom)] & (~figures);

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                moves.push_back({positionFrom, positionTo});

                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }
        bbFrom = (color == WHITE) ? wPawns : bPawns;
        while (bbFrom) {
            bitboard positionFrom = (bbFrom & -bbFrom);
            bitboard bitNumber = BitBoard::bitNumberFromBitBoard(positionFrom);
            bitboard bbTo = (color == WHITE) ? WHITE_PAWNS_SHORT_MOVES[bitNumber] & (~figures)
                                             : BLACK_PAWNS_SHORT_MOVES[bitNumber] & (~figures);
            int numberRow = (bitNumber >> 3);

            while (bbTo) {
                bitboard positionTo = (bbTo & -bbTo);
                moves.push_back({positionFrom, positionTo});
                if (numberRow == 1 && color == BLACK || numberRow == 6 && color == WHITE) {
                    bitboard positionTo2 = (color == WHITE) ? WHITE_PAWNS_LONG_MOVES[bitNumber] & (~figures) : BLACK_PAWNS_LONG_MOVES[bitNumber] & (~figures);
                    if (positionTo2) {
                        moves.push_back({positionFrom, positionTo2});
                    }
                }
                bbTo ^= positionTo;
            }

            bbFrom ^= positionFrom;
        }

        return moves;

    }

    void alphabeta(int color, int depth, int alpha, int beta) {
        visitedPositionsCnt[depth] ++;
        list<move> moves = generateMoves(color);

        for (auto mv: moves) {

            doMove(mv);
            if (depth != 0) {
                alphabeta(color ^ WHITE_BLACK, depth - 1, -beta, -alpha);
            }
            undoMove(mv);
        }

    }

    vector<ll> getVisitedPositionsCnt() {
       return visitedPositionsCnt;
    }

}