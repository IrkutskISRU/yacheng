#include <iostream>
#include <string>

#include "constants.h"
#include "engine.h"
#include "board.h"
#include "perft.h"
#include "bitboard_precalc.h"
#include "precalc.h"

int main() {

    BitBoard::initDecode();
    Constants::init();

//    EPD position("1n4kr/2B4p/2nb2b1/ppp5/P1PpP3/3P4/5K2/1N1R4 b - c3");
//    Engine::init(position);
//    position.print();

    testPerft();

    return 0;
}