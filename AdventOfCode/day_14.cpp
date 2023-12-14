// day_14.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_14.test_input";  // Part 1: 136, part 2: 64
// static const char* input_file_name = "day_14.input";  // Part 1: 108826, part 2:

static const u32 kMaxRows = 100;
static const u32 kMaxCols = 100;

// static const u32 kCycles = 1;
static const u32 kCycles = 1000000000;


struct Grid
{
    u32 rows;
    u32 cols;
    char items[kMaxRows][kMaxCols];
};


void PrintGrid(Grid* grid)
{
    for (int row = 0; row < grid->rows; ++row) {
        for (int col = 0; col < grid->cols; ++col) {
            fprintf(stdout, "%c", grid->items[row][col]);
        }
        fprintf(stdout, "\n");
    }
}


void TiltGridNorth(Grid* grid)
{
    for (int col = 0; col < grid->cols; ++col) {
        i32 adjust = 0;
        for (int row = 0; row < grid->rows; ++row) {
            char c = grid->items[row][col];
            switch (c) {
                case '.': {
                    adjust -= 1;
                    break;
                }
                case 'O': {
                    if (adjust < 0) {
                        i32 new_row = row + adjust;
                        grid->items[new_row][col] = 'O';
                        grid->items[row][col] = '.';
                    }
                    break;
                }
                case '#': {
                    adjust = 0;
                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }
        }
    }
}

void TiltGridSouth(Grid* grid)
{
    for (int col = 0; col < grid->cols; ++col) {
        i32 adjust = 0;
        for (int row = (grid->rows - 1); row >= 0; --row) {
            char c = grid->items[row][col];
            switch (c) {
                case '.': {
                    adjust += 1;
                    break;
                }
                case 'O': {
                    if (adjust > 0) {
                        i32 new_row = row + adjust;
                        grid->items[new_row][col] = 'O';
                        grid->items[row][col] = '.';
                    }
                    break;
                }
                case '#': {
                    adjust = 0;
                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }
        }
    }
}


void TiltGridEast(Grid* grid)
{
    for (int row = 0; row < grid->rows; ++row) {
        i32 adjust = 0;
        for (int col = (grid->cols - 1); col >= 0; --col) {
            char c = grid->items[row][col];
            switch (c) {
                case '.': {
                    adjust += 1;
                    break;
                }
                case 'O': {
                    if (adjust > 0) {
                        i32 new_col = col + adjust;
                        grid->items[row][new_col] = 'O';
                        grid->items[row][col] = '.';
                    }
                    break;
                }
                case '#': {
                    adjust = 0;
                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }
        }
    }
}


void TiltGridWest(Grid* grid)
{
    for (int row = 0; row < grid->rows; ++row) {
        i32 adjust = 0;
        for (int col = 0; col < grid->cols; ++col) {
            char c = grid->items[row][col];
            switch (c) {
                case '.': {
                    adjust -= 1;
                    break;
                }
                case 'O': {
                    if (adjust < 0) {
                        i32 new_col = col + adjust;
                        grid->items[row][new_col] = 'O';
                        grid->items[row][col] = '.';
                    }
                    break;
                }
                case '#': {
                    adjust = 0;
                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }
        }
    }
}


u32 ScoreGrid(Grid* grid)
{
    u32 result = 0;
    
    for (int row = 0; row < grid->rows; ++row) {
        u32 row_count = 0;
        for (int col = 0; col < grid->cols; ++col) {
            if (grid->items[row][col] == 'O') {
                row_count++;
            }
        }
        result += row_count * (grid->rows - row);
    }
    
    return result;
}


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
    
    // PrintGrid(&grid);
    
    for (int cycle = 0; cycle < kCycles; ++cycle) {
        TiltGridNorth(&grid);
        TiltGridWest(&grid);
        TiltGridSouth(&grid);
        TiltGridEast(&grid);
        if (cycle % 1000000 == 0) {
            fprintf(stdout, "Cycles %u\n", cycle);
        }
    }
    
    // fprintf(stdout, "\n\n");
    // PrintGrid(&grid);
    
    u32 score = ScoreGrid(&grid);
    fprintf(stdout, "Part 1 score: %u\n", score);

    CloseFile(&input_file);
}


