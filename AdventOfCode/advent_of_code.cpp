// advent_of_code.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


bool MatchNumberKeyword(const char* line, int line_index, KeyWord keyword)
{
    bool match = true;
    
    for (int index = 0; index < keyword.length; ++index, ++line_index) {
        if (line[line_index] != keyword.start[index]) {
            match = false;
            break;
        }
    }
    
    return match;
}
