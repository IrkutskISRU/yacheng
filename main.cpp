#include <iostream>
#include <string>

#include "engine.h"
#include "board.h"
#include "constants.h"
int main() {

    EPD position("1n4kr/2B4p/2nb2b1/ppp5/P1PpP3/3P4/5K2/1N1R4 b - c3");
    position.print();

    return 0;
}