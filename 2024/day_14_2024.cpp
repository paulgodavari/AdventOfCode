// day_14_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


// static const char* input_file_name = "../../2024/input/day_14.test_input";  // Part 1 = 12, Part 2 =
static const char* input_file_name = "../../2024/input/day_14.input";  // Part 1 = 218965032, Part 2 =


struct Position
{
    i32 row;
    i32 col;
};


struct Grid
{
    i32 rows;
    i32 cols;
};


Position ComputeFinalPosition(Grid grid, Position position, Position velocity, i32 seconds)
{
    Position result = { position.row + seconds * velocity.row, position.col + seconds * velocity.col };
    result.row = result.row % grid.rows;
    result.col = result.col % grid.cols;
    
    if (result.row < 0) {
        result.row = grid.rows + result.row;
    }
    
    if (result.col < 0) {
        result.col = grid.cols + result.col;
    }
    
    return result;
}


void PrintCount(Grid grid, u32* count)
{
    for (int row = 0; row < grid.rows; ++row) {
        for (int col = 0; col < grid.cols; ++col) {
            u32 value = *(count + row * grid.cols + col);
            if (value) {
                fprintf(stdout, "%u", value);
            } else {
                fprintf(stdout, ".");
            }
        }
        fprintf(stdout, "\n");
    }
}


u32 ComputeBots(u32* count, Grid grid, Position start, Position stop)
{
    u32 result = 0;

    for (int row = start.row; row < stop.row; ++row) {
        for (int col = start.col; col < stop.col; ++col) {
            u32 value = *(count + row * grid.cols + col);
            result += value;
        }
    }

    return result;
}


void Day14_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    u64 part1_answer = 0;
    u64 part2_answer = 0;
    
    // const Grid grid = { 7, 11 };
    const Grid grid = { 103, 101 };
    
    u32 count[grid.rows][grid.cols] = {};

    
    while (!AtEndOfFile(&parser)) {
        Position position;
        ConsumeString(&parser, CONST_STRING("p="));
        position.col = (i32) ParseNumber(&parser);
        Advance(&parser);  // Comma
        position.row = (i32) ParseNumber(&parser);
        
        Position velocity;
        ConsumeString(&parser, CONST_STRING(" v="));
        velocity.col = (i32) ParseSignedNumber(&parser);
        Advance(&parser);  // Comma
        velocity.row = (i32) ParseSignedNumber(&parser);
        Advance(&parser);
        
        Position final = ComputeFinalPosition(grid, position, velocity, 100);
        fprintf(stdout, "Start: (%d, %d), Final: (%d, %d)\n", position.col, position.row, final.col, final.row);
        
        count[final.row][final.col] += 1;
    }
    
    // PrintCount(grid, &count[0][0]);
    
    Position start = { 0, 0 };
    Position stop = { grid.rows / 2, grid.cols / 2 };
    u32 quad0 = ComputeBots(&count[0][0], grid, start, stop);

    start = { 0, grid.cols / 2 + 1 };
    stop = { grid.rows / 2, grid.cols };
    u32 quad1 = ComputeBots(&count[0][0], grid, start, stop);

    start = { grid.rows / 2 + 1, 0 };
    stop = { grid.rows, grid.cols / 2 };
    u32 quad3 = ComputeBots(&count[0][0], grid, start, stop);

    start = { grid.rows / 2 + 1, grid.cols / 2 + 1 };
    stop = { grid.rows, grid.cols };
    u32 quad2 = ComputeBots(&count[0][0], grid, start, stop);

    part1_answer = quad0 * quad1 * quad2 * quad3;

    fprintf(stdout, "2024: Day 13 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2024: Day 13 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
