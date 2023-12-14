// day_14.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_14.test_input";  // Part 1: , part 2:
// static const char* input_file_name = "day_14.input";  // Part 1: , part 2:

static const u32 kMaxRows = 100;
static const u32 kMaxCols = 100;


struct Grid
{
    u32 rows;
    u32 cols;
    char items[kMaxRows][kMaxCols];
};


void Day14()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    fprintf(stdout, "Day 14\n");
    
    ParseState parser = { input_file.data, input_file.size, 0 };

    Grid grid = {};
    
    while (!AtEndOfFile(&parser)) {
        u32 columns = 0;
        while (!AtEndOfLine(&parser)) {
            char c = parser.data[parser.offset];
            grid.items[grid.rows][columns] = c;
            columns++;
            Advance(&parser);
        }
        Advance(&parser);
        grid.rows++;
        if (grid.cols < columns) {
            grid.cols = columns;
        }
        columns = 0;
    }
    
    fprintf(stdout, "Rows: %u, cols: %u\n", grid.rows, grid.cols);
    
    for (int row = 0; row < grid.rows; ++row) {
        for (int col = 0; col < grid.cols; ++col) {
            fprintf(stdout, "%c", grid.items[row][col]);
        }
        fprintf(stdout, "\n");
    }
    
    CloseFile(&input_file);
}


