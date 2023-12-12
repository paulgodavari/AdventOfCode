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


void Day11()
{
    fprintf(stdout, "Day 11\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading file: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    u32 galaxy_count = 0;
    Position galaxy_map[kMaxGalaxies] = {};
    
    bool row_has_galaxy[kMaxRows] = {};
    bool col_has_galaxy[kMaxCols] = {};
    
    i32 rows = 0;
    i32 cols = 0;
    while (!AtEndOfFile(&parser)) {
        cols = 0;
        while (!AtEndOfLine(&parser)) {
            char current_char = parser.data[parser.offset];
            if (current_char == '#') {
                galaxy_map[galaxy_count] = { rows, cols };
                galaxy_count++;
                col_has_galaxy[cols] = true;
                row_has_galaxy[rows] = true;
            }
            cols++;
            Advance(&parser);
        }
        rows++;
        Advance(&parser);
    }
    // u32 cols = (u32) parser.size / rows;
    
    fprintf(stdout, "Galaxy count: %u (r: %u, c: %u)\n", galaxy_count, rows, cols);
    
    i32 empty_row_count = 0;
    fprintf(stdout, "Empty rows:  ");
    for (int i = 0; i < rows; ++i) {
        if (!row_has_galaxy[i]) {
            empty_row_count++;
            fprintf(stdout, "%d ", i);
        }
    }
    fprintf(stdout, " (%d)\n", empty_row_count);

    i32 empty_col_count = 0;
    fprintf(stdout, "Empty cols:  ");
    for (int i = 0; i < cols; ++i) {
        if (!col_has_galaxy[i]) {
            empty_col_count++;
            fprintf(stdout, "%d ", i);
        }
    }
    fprintf(stdout, " (%d)\n", empty_col_count);

    CloseFile(&input_file);
}

