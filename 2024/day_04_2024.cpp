// day_04_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


// static const char* input_file_name = "../../2024/input/day_04.test_input";  // Part 1 =  18, Part 2 = 9
static const char* input_file_name = "../../2024/input/day_04.input";  // Part 1 = 2514, Part 2 = 1888


struct Grid
{
    ParseState* parser;
    u32 rows;
    u32 cols;
};


struct Direction
{
    i32 x;
    i32 y;
};


struct Slash
{
    Direction dir[2];
};


Grid ComputeDimentions(ParseState* parser)
{
    Grid result = { parser, 0, 0 };
    
    u32 index = 0;
    while (parser->data[index++] != '\n') {
        result.cols++;
    }
    result.cols++;  // Need to include the '\n' in our calculations.
    result.rows = (u32) (parser->size / result.cols);
    
    return result;
}


bool ValidPosition(Grid* grid, u32 row, u32 col)
{
    return (row >= 0 && row < grid->rows) && (col >= 0 && col < grid->cols - 1);
}


char CharAtPosition(Grid* grid, u32 row, u32 col)
{
    char result = 0;
    
    u32 index = row * grid->cols + col;
    if (index < grid->parser->size) {
        result = grid->parser->data[index];
    }
    
    return result;
}


u32 SearchDirection(Grid* grid, u32 row, u32 col, Direction dir)
{
    u32 result = 0;
    
    row += dir.y;
    col += dir.x;
    if (ValidPosition(grid, row, col) && (CharAtPosition(grid, row, col) == 'M')) {
        row += dir.y;
        col += dir.x;
        if (ValidPosition(grid, row, col) && (CharAtPosition(grid, row, col) == 'A')) {
            row += dir.y;
            col += dir.x;
            if (ValidPosition(grid, row, col) && (CharAtPosition(grid, row, col) == 'S')) {
                result = 1;
            }
        }
    }
    
    return result;
}


u32 SearchAllDirections(Grid* grid, u32 row, u32 col)
{
    u32 result = 0;
    
    Direction all[] = {
        {  0, -1 },
        {  1, -1 },
        {  1,  0 },
        {  1,  1 },
        {  0,  1 },
        { -1,  1 },
        { -1,  0 },
        { -1, -1 }
    };
    
    u32 direction_count = sizeof(all) / sizeof(all[0]);
    
    for (int index = 0; index < direction_count; ++index) {
        Direction dir = all[index];
        result += SearchDirection(grid, row, col, dir);
    }
    
    return result;
}


u32 CountMatches(Grid* grid, u32 row, u32 col)
{
    u32 result = 0;
    
    if (ValidPosition(grid, row, col)) {
        if (CharAtPosition(grid, row, col) == 'X') {
            result = SearchAllDirections(grid, row, col);
        }
    }
    
    return result;
}


bool SearchSlashChar(Grid* grid, u32 row, u32 col, Slash slash, char c1, char c2)
{
    bool result = false;

    Direction dir = slash.dir[0];
    u32 x = col + dir.x;
    u32 y = row + dir.y;
    if (ValidPosition(grid, y, x) && (CharAtPosition(grid, y, x) == c1)) {
        dir = slash.dir[1];
        x = col + dir.x;
        y = row + dir.y;
        if (ValidPosition(grid, y, x) && (CharAtPosition(grid, y, x) == c2)) {
            result = true;
        }
    }

    return result;
}


bool SearchSlash(Grid* grid, u32 row, u32 col, Slash slash)
{
    bool result = false;

    if (SearchSlashChar(grid, row, col, slash, 'M', 'S') || SearchSlashChar(grid, row, col, slash, 'S', 'M')) {
        result = true;
    }
    
    return result;
}


u32 SearchStarDirections(Grid* grid, u32 row, u32 col)
{
    u32 result = 0;

    Slash forward_slash  = {{ { -1,  1 }, {  1, -1 } }};
    Slash backward_slash = {{ { -1, -1 }, {  1,  1 } }};

    if (SearchSlash(grid, row, col, forward_slash) && SearchSlash(grid, row, col, backward_slash)) {
        result = 1;
    }
    
    return result;
}


u32 CountMASMatches(Grid* grid, u32 row, u32 col)
{
    u32 result = 0;
    
    if (ValidPosition(grid, row, col) && (CharAtPosition(grid, row, col) == 'A')) {
        result = SearchStarDirections(grid, row, col);
    }
    
    return result;
}


void Day04_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    Grid grid = ComputeDimentions(&parser);
    
    u32 part1_count = 0;
    for (int row = 0; row < grid.rows; ++row) {
        for (int col = 0; col < grid.cols - 1; ++col) {
            part1_count += CountMatches(&grid, row, col);
        }
    }
    
    u32 part2_count = 0;
    for (int row = 0; row < grid.rows; ++row) {
        for (int col = 0; col < grid.cols - 1; ++col) {
            part2_count += CountMASMatches(&grid, row, col);
        }
    }

    fprintf(stdout, "2024: Day 04 part 1: %u\n", part1_count);
    fprintf(stdout, "2024: Day 04 part 2: %u\n", part2_count);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
