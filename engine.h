//
// Created by artyom-m on 3/20/19.
//
#pragma once

#include <vector>

#include "types.h"
#include "board.h"
#include "constants.h"
#include "config.h"

namespace Engine {

    struct move {
        int figure;
        bitboard from;
        bitboard to;
        bool enPassant;
        int newFigure;

        move(int Figure, bitboard From, bitboard To, bool EnPassant = false, int NewFigure = 0) :
                figure(Figure),
                from(From),
                to(To),
                enPassant(EnPassant),
                newFigure(NewFigure)
        {
        }
    };

    void init(EPD &position);
    void alphabeta(int color, int depth, int alpha, int beta);
    inline void doMove(move mv, int color);
    inline void undoMove(move mv, int color, int chopped, bitboard oldEnPassant, bitboard oldFacticalPawn);
    vector<ll> getVisitedPositionsCnt();
    vector<int>getBoard();
    void printBoard();
    int getMark();
}