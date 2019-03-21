//
// Created by artyom-m on 3/21/19.
//

#include <string>
#include <vector>
#include "board.h"

struct testCase {
    string name;
    EPD position;
    vector <ll> v;

    void print();
};

void testPerft();