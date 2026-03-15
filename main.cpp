#include <iostream>
#include <string>
#include <chrono>

#include "constants.h"
#include "engine.h"
#include "bitboard.h"
#include "board.h"
#include "perft.h"
#include "bitboard_precalc.h"
#include "precalc.h"
#include "config.h"

int main() {
    BitBoard::initDecode();
    Constants::init();

    if (mode == engineMode::Launch) {
	    EPD position("rnbqkbnr/p1ppppPp/8/8/8/8/PPpP1PPP/RNBQKBNR w KQkq - 0 1");
	    Engine::init(position);
	    position.print();
	    vector<Engine::move> moves;
        Engine::move tmpMove{100,100,100};
	    Engine::alphabeta(WHITE, 0, 7, -oo, oo, moves, 0, tmpMove);
	    cout << "\n---\n";
	    position.print();
	    cout << Engine::getMark(WHITE);
    } else if (mode == engineMode::Perft) {
        testPerft();
        return 0;
    } else if (mode == engineMode::Game) {

        int currentMove = 0;

	    srand(time(0));

	    while (true) {
		string s;
		cin >> s;
		cout << s << "\n";
		if (s == "uci") {
		    cout << "id name YaCheng 0.0\n";
		    cout << "id author Artyom Mukhometzyanov\n";
		    cout << "uciok\n";
		}
		if (s == "quit") {
		    break;
		}

		if (s == "isready") {
		    cout << "readyok\n";
		}

		if (s == "moves") {

		    EPD position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
		    Engine::init(position);
            currentMove = 0;
		    int color = WHITE;
		    do {
			cin >> s;
			if (s != "go") {

                currentMove ++;

			    bitboard from = (1ull << (8 * ('8' - s[1]) + (s[0] - 'a')));
			    bitboard to = (1ull << (8 * ('8' - s[3]) + (s[2] - 'a')));
			    int figure = Engine::getBoard()[(8 * ('8' - s[1]) + (s[0] - 'a'))];
			    int enemyFigure = Engine::getBoard()[(8 * ('8' - s[3]) + (s[2] - 'a'))];
			    Engine::move mv{figure, from, to};
			    if (s == "e1g1" && figure == WHITE_KING) {
				mv.figure = 100;
			    }
			    if (s == "e1c1" && figure == WHITE_KING) {
				mv.figure = 200;
			    }
			    if (s == "e8g8" && figure == BLACK_KING) {
				mv.figure = 300;
			    }
			    if (s == "e8c8" && figure == BLACK_KING) {
				mv.figure = 400;
			    }
			    mv.enPassant = false;
			    mv.newFigure = 0;
			    if (s.length() > 4 && s[4] == 'q') {
				mv.newFigure = color == WHITE ? WHITE_QUEEN : BLACK_QUEEN;
			    }
			    if (s.length() > 4 && s[4] == 'r') {
				mv.newFigure = color == WHITE ? WHITE_ROOK : BLACK_ROOK;
			    }
			    if (s.length() > 4 && s[4] == 'b') {
				mv.newFigure = color == WHITE ? WHITE_BISHOP : BLACK_BISHOP;
			    }
			    if (s.length() > 4 && s[4] == 'n') {
				mv.newFigure = color == WHITE ? WHITE_KNIGHT : BLACK_KNIGHT;
			    }

			    if ((figure & PAWN) && s[0] != s[2] && !enemyFigure) {
				mv.enPassant = true;
			    }


			    Engine::doMove(mv, color, currentMove);
	//                    Engine::print_board();
			}
			color = color ^ WHITE_BLACK;

		    } while (s != "go");

		    if (s == "go") {
	//                Engine::print_board();
            Engine::move bestMove{100, 100, 100};
			auto start = std::chrono::high_resolution_clock::now();
			for (int x = 3; x <= 11; x++) {
				vector<Engine::move> moves;
				cout << "depth: " << x << "\n";
				auto mark = Engine::alphabeta(color ^ WHITE_BLACK, 0, x, -oo, oo, moves, currentMove, bestMove);
				cout <<"mark: " << mark << "\n";			
				auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> duration = end - start;
				cout << "duration: ";
				cout << duration.count() << "\n";
}

		    }

		} else

		if (s == "go") {
		    cout << "bestmove e2e4\n";
		}
	    }
    }


    return 0;
}
