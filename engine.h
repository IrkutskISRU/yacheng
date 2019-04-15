//
// Created by artyom-m on 3/20/19.
//
#pragma once

#include <vector>

#include "types.h"
#include "board.h"
#include "constants.h"

namespace Engine {
    void init(EPD &position);
    void alphabeta(int color, int depth, int alpha, int beta);
    vector<ll> getVisitedPositionsCnt();
}