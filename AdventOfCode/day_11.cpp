// day_11.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_11.test_input";  // Part 1: 374, part 2:
// static const char* input_file_name = "day_11.input";  // Part 1: , part 2:


static const u32 kMaxGalaxies = 512;
static const u32 kMaxRows = 140;
static const u32 kMaxCols = 140;  // 141 if including the '\n' character.


struct Position
{
    i32 row;
    i32 col;
};

const Position kPositionInvalid = { -1, -1 };


void Day11()
{
    fprintf(stdout, "Day 11\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading file: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    u32 rows = 0;
    u32 galaxy_count = 0;
    while (!AtEndOfFile(&parser)) {
        while (!AtEndOfLine(&parser)) {
            char current_char = parser.data[parser.offset];
            if (current_char == '#') {
                galaxy_count++;
            }
            Advance(&parser);
        }
        rows++;
        Advance(&parser);
    }
    u32 cols = (u32) parser.size / rows;

    
    fprintf(stdout, "Galaxy count: %u (r: %u, c: %u)\n", galaxy_count, rows, cols);
    
    CloseFile(&input_file);
}

