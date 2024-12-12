// day_12_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <unordered_set>


// static const char* input_file_name = "../../2024/input/day_12.test_input";  // Part 1 = 1930, Part 2 = 
static const char* input_file_name = "../../2024/input/day_12.input";  // Part 1 = , Part 2 = 


struct Grid
{
    ParseState* parser;
    i32 rows;
    i32 cols;  // Includes newline
};


struct Position
{
    i32 row;
    i32 col;
    
    bool operator==(const Position& other) const {
        return col == other.col && row == other.row;
    }
};


namespace std {
    template <>
    struct hash<Position> {
        std::size_t operator()(const Position& p) const {
            return std::hash<int>()(p.col) ^ (std::hash<int>()(p.row) << 1);
        }
    };
}


static Grid ComputeGrid(ParseState* parser)
{
    Grid result = { parser, 0, 0 };
    
    while (parser->data[result.cols] != '\n') {
        result.cols++;
    }
    result.cols++;  // Include the newline in calculations.
    result.rows = (i32) parser->size / result.cols;

    return result;
}


static Position PositionFromIndex(Grid* grid, u32 offset)
{
    Position result = { -1, -1 };
    
    if (offset < grid->parser->size) {
        result.row = offset / grid->cols;
        result.col = offset % grid->cols;
    }
    
    return result;
}


static u32 OffsetFromPosition(Grid* grid, Position position)
{
    u32 result = position.row * grid->cols + position.col;
    return result;
}


static bool IsValidPosition(Grid* grid, Position position)
{
    return (position.col >= 0 && position.col < grid->cols - 1) && (position.row >= 0 && position.row < grid->rows);
}


static i32 ValueAtPosition(Grid* grid, Position position)
{
    i32 result = -1;
    
    u32 offset = OffsetFromPosition(grid, position);
    if (offset < grid->parser->size) {
        char current = grid->parser->data[offset];
        if (current >= '0' && current <= '9') {
            result = current - '0';
        }
    }
    
    return result;
}




void Day12_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    // Search for each trailhead (char == '0') and recursively search it.
    u64 part1_answer = 0;
    u64 part2_answer = 0;

    fprintf(stdout, "2024: Day 10 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2024: Day 10 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
