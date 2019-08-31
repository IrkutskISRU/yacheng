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


const int numTests = 5;
const auto& perftFileName = "new_perft.txt";
testCase tests[numTests];

void testPerft() {
    ifstream in(perftFileName);

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

    int passed = 0;
    int notPassed = 0;
    int D = 4;
    for (int i = 0; i < numTests; i++) {

        Engine::init(tests[i].position);

        Engine::alphabeta(tests[i].position.color, D, -oo, oo);
        int j = 0;
        bool OK = true;

        auto visitedPositions = Engine::getVisitedPositionsCnt();
        bool wasFirstNotNull = false;
        for (auto it = visitedPositions.rbegin(); it != visitedPositions.rend(); it++) {
            if (*it > 0) {
                if (wasFirstNotNull) {
                    cout << i << " " << *it << " ";

                    if (*it != tests[i].v[j]) {
                        OK = false;
                    }
                    j++;

                }
                wasFirstNotNull = true;
            }
        }
        if (OK) {
            cout << "OK ";
            passed ++;
        } else {
            notPassed ++;
            cout << "\nWA\n";
            tests[i].print();
            cout <<"VISITED POSITIONS: ";
            wasFirstNotNull = false;
            for (auto it = ++visitedPositions.rbegin(); it != visitedPositions.rend(); it++) {
                if (*it > 0) {
                    if (wasFirstNotNull) {
                    cout << *it << " ";
                        j++;

                    }
                    wasFirstNotNull = true;
                }
            }
            cout << "\n";

        }
        if (i % 10 == 9) {
            cout.flush();
        }
        if (i % 100 == 99) {
            cout << "\nPassed: " << passed << " Not Passed: " << notPassed << "\n";
        }
    }

    cout << "\nPassed: " << passed << " Not Passed: " << notPassed << "\n";


}
