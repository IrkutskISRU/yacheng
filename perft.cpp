//
// Created by artyom-m on 3/21/19.
//

#include "perft.h"
#include "board.h"
#include "engine.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


void testCase::print() {
    cout << "========================\n";
    cout << "TESTCASE: " << name << "\n";
    position.print();
    cout << "ANSWERS          : ";
    for (auto item: v)
        cout << item << " ";
    cout << "\n";
}


const int numTests = 6383;
testCase tests[numTests];

void testPerft() {
    ifstream in("perft.txt");

    for (int j = 0; j < numTests; j++) {
        string testName;
        in >> testName;
        string str1, str2, str3, str4;
        in >> str1 >> str2 >> str3 >> str4;

        EPD position(str1 + " " + str2 + " " + str3 + " " + str4);

        vector <ll> res;
        for (int i = 0; i < 6; i++) {
            ll item;
            in >> item;
            res.push_back(item);
        }

        tests[j] = {testName, position, res};
    }


    for (int i = 0; i < 1; i++) {

        Engine::init(tests[i].position);
//        if (!(tests[i].position.castleq)) continue;
        tests[i].print();
        Engine::alphabeta(tests[i].position.color, 7, -oo, oo);

        cout << "Visited Positions: ";
        auto visitedPositions = Engine::getVisitedPositionsCnt();
        bool wasFirstNotNull = false;
        for (auto it = visitedPositions.rbegin(); it != visitedPositions.rend(); it++) {
            if (*it > 0) {
                if (wasFirstNotNull) {
                    cout << *it << " ";
                }
                wasFirstNotNull = true;
            }
        }
        cout << "\n";
    }


}
