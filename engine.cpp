//
// Created by artyom-m on 3/21/19.
//

#define INIT(variable) variable = position.variable;

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
        if (wKings & mv.from) {
            wKings ^= mv.from;
            wKings ^= mv.to;

            wFigures ^= mv.from;
            wFigures ^= mv.to;

            figures ^= mv.from;
            figures ^= mv.to;
        }
        if (bKings & mv.from) {
            bKings ^= mv.from;
            bKings ^= mv.to;

            bFigures ^= mv.from;
            bFigures ^= mv.to;

            figures ^= mv.from;
            figures ^= mv.to;
        }
    }

    inline void undoMove(move mv) {
        if (wKings & mv.to) {
            wKings ^= mv.from;
            wKings ^= mv.to;

            wFigures ^= mv.from;
            wFigures ^= mv.to;

            figures ^= mv.from;
            figures ^= mv.to;
        }
        if (bKings & mv.to) {
            bKings ^= mv.from;
            bKings ^= mv.to;

            bFigures ^= mv.from;
            bFigures ^= mv.to;

            figures ^= mv.from;
            figures ^= mv.to;
        }
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

        if (color == WHITE) {
            bitboard bbFrom = wKings;
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

        } else {
            bitboard bbFrom = bKings;
            while (bbFrom) {
                bitboard positionFrom = (bbFrom & -bbFrom);
                bitboard bbTo = KING_MOVES[BitBoard::bitNumberFromBitBoard(positionFrom)] & (~figures);

                while (bbTo) {
                    bitboard positionTo = (bbTo & -bbTo);
                    moves.push_back({positionFrom, positionTo});
//                    cout << bKings << "\n";
//                    cout << BitBoard::bitNumberFromBitBoard(positionFrom) << "/" << BitBoard::bitNumberFromBitBoard(positionTo) << "|";

                    bbTo ^= positionTo;
                }

                bbFrom ^= positionFrom;
            }
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