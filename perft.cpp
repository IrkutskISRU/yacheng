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
    cout << "ANSWERS: ";
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


    for (int i = 0; i < 10; i++) {
        tests[i].print();
        Engine::init(tests[i].position);
        Engine::alphabeta(tests[i].position.color,6,-oo, oo);
        cout << "Visited Positions " << Engine::getVisitedPositionsCnt() << "\n";
    }


}
