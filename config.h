//
// Created by artyom-m on 8/31/19.
//
#pragma once
#ifndef CHESS_ENGINE_CONFIG_H
#define CHESS_ENGINE_CONFIG_H

#endif //CHESS_ENGINE_CONFIG_H

enum engineMode {
    Perft = 1,
    Game = 2,
    Launch = 3
};

const engineMode mode = engineMode::Game;
