cmake_minimum_required(VERSION 3.10)
project(chess_engine)

set(CMAKE_CXX_STANDARD 11)

add_executable(chess_engine main.cpp board.cpp board.h engine.h types.h engine.cpp constants.h constants.cpp perft.h perft.cpp bitboard.cpp bitboard.h bitboard_precalc.h precalc.cpp)