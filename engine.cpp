//
// Created by artyom-m on 3/21/19.
//

#define INIT(variable) variable = position.variable;

#include "engine.h"
#include <list>

struct figure {
    int type;
    bitboard pos;
};

namespace Engine {
    bitboard wKings, wQueens, wRooks, wBishops, wKnights, wPawns;
    bitboard bKings, bQueens, bRooks, bBishops, bKnights, bPawns;
    bitboard wFigures, bFigures, figures;

    int color;
    ll visitedPositionsCnt = 0;
    bool castleQ, castleK, castleq, castlek;
    string enPassant;

    std::list<figure> figuresList;

    struct move {
        bitboard from;
        bitboard to;
    };

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
        INIT(color);
        INIT(castleQ);
        INIT(castleK);
        INIT(castleq);
        INIT(castlek);
        INIT(enPassant);

        wFigures = (wKings | wQueens | wRooks | wBishops | wKnights | wPawns);
        bFigures = (bKings | bQueens | bRooks | bBishops | bKnights | bPawns);
        figures = (bFigures | wFigures);

        visitedPositionsCnt = 0;
    };

    inline void generateMoves() {
        visitedPositionsCnt ++;
        if (color == WHITE) {

        }

    }

    void alphabeta(int color, int depth, int alpha, int beta) {

        vector
        generateMoves();

    }

    ll getVisitedPositionsCnt() {
       return visitedPositionsCnt;
    }



}