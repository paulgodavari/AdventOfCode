// day_14.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <unordered_map>
#include <vector>


static const char* input_file_name = "day_14.test_input";  // Part 1: 136, part 2: 64
// static const char* input_file_name = "day_14.input";  // Part 1: 108826, part 2: 99291

static const u32 kMaxRows = 100;
static const u32 kMaxCols = 100;

static const u32 kCycles = 1000000000;


struct Grid
{
    u32 rows;
    u32 cols;
    char items[kMaxRows][kMaxCols];
    
    bool operator==(const Grid& rhs) const
    {
        return memcmp(items, rhs.items, kMaxCols * kMaxRows * sizeof(char)) == 0;
    }
};


struct GridHash
{
    std::size_t operator()(const Grid& grid) const
    {
        std::hash<char> hasher;
        std::size_t hash = 0;
        for (int row = 0; row < grid.rows; ++row) {
            for (int col = 0; col < grid.cols; ++col) {
                hash ^= hasher(grid.items[row][col]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
        }
        return hash;
    }
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
    // fprintf(stdout, "\n\n");
    // TiltGridNorth(&grid);
    // u32 score = ScoreGrid(&grid);
    // fprintf(stdout, "Part 1 score: %u\n", score);
    // PrintGrid(&grid);
    
    std::unordered_map<Grid, u32, GridHash> grid_map;
    std::vector<u32> scores;
    
    for (int cycle = 0; cycle < kCycles; ++cycle) {
        TiltGridNorth(&grid);
        TiltGridWest(&grid);
        TiltGridSouth(&grid);
        TiltGridEast(&grid);
        
        auto it = grid_map.find(grid);
        if (it != grid_map.end()) {
            u32 found_cycle = it->second;
            u32 period = cycle - found_cycle;
            u32 index = (kCycles - found_cycle) % period;
            u32 offset = found_cycle - 1 + index;
            u32 final_score = scores[offset];
            fprintf(stdout, "Day 14 part 2: final_score: %u (period: %u)\n", final_score, period);
            break;
        } else {
            u32 score = ScoreGrid(&grid);
            scores.push_back(score);
            grid_map[grid] = cycle;
        }

        if (cycle % 1000000 == 0) {
            fprintf(stdout, "Cycles %u\n", cycle);
        }
    }
    

    CloseFile(&input_file);
}


