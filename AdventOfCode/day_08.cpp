// day_08.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_08.test_input";  // Part 1: , Part 2:
// static const char* input_file_name = "day_08.input";  // Part 1: , Part 2:


void Day08()
{
    fprintf(stdout, "Day 08\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    String directions = ParseWord(&parser);
    Advance(&parser, 2);
    
    u32 line_count = 0;
    while (!AtEndOfFile(&parser)) {
        String index = ParseWord(&parser);
        assert(index.size == 3);
        Advance(&parser, 4);
        String left = ParseWord(&parser);
        Advance(&parser, 2);
        String right = ParseWord(&parser);
        Advance(&parser);
        assert(AtEndOfLine(&parser));
        
        fprintf(stdout, "%u: %.*s -> [%.*s, %.*s]\n",
                line_count,
                (int) index.size, index.start,
                (int) left.size, left.start,
                (int) right.size, right.start);

        line_count++;
        Advance(&parser);
    }
}


