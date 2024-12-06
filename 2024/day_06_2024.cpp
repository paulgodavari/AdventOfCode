// day_06_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <unordered_map>
#include <vector>


static const char* input_file_name = "../../2024/input/day_06.test_input";  // Part 1 = 41, Part 2 =
// static const char* input_file_name = "../../2024/input/day_06.input";  // Part 1 = , Part 2 =


enum class Direction
{
    Invalid = 0,
    Up,
    Right,
    Down,
    Left
};


struct Position
{
    i32 x;
    i32 y;
};


struct Move
{
    Position position;
    Direction direction;
};


struct Grid
{
    ParseState* parser;
    i32 rows;
    i32 cols;
};


Grid ComputeGridDimensions(ParseState* parser)
{
    Grid result = { parser };
    
    u32 cols = 0;
    while (parser->data[cols] != '\n' && cols < parser->size) {
        cols++;
    }
    
    result.cols = cols + 1;  // Include '\n' in calculations.
    result.rows = (i32) parser->size / result.cols;
    
    return result;
}


Position IndexToGridPosition(Grid* grid, u32 index)
{
    Position result = {};
    
    if (index >= 0 && index < grid->parser->size) {
        result.y = index / grid->cols;
        result.x = index % grid->cols;
    }
    
    return result;
}


i32 GridPositionToIndex(Grid* grid, Position position)
{
    i32 result = position.y * grid->cols + position.x;
    return result;
}


bool PositionIsInGrid(Grid* grid, Position position)
{
    bool result = false;
    
    if (position.x >= 0 && position.x < grid->cols - 1 &&
        position.y >= 0 && position.y < grid->rows) {
        result = true;
    }
    
    return result;
}


Direction GetNextDirection(Direction direction)
{
    Direction result = {};
    
    switch (direction) {
        case Direction::Up:
            result = Direction::Right;
            break;
        case Direction::Right:
            result = Direction::Down;
            break;
        case Direction::Down:
            result = Direction::Left;
            break;
        case Direction::Left:
            result = Direction::Up;
            break;
        default:
            break;
    }
    
    return result;
}


Move FindNextMove(Grid* grid, Move last_move)
{
    Move result = {};
    
    Position next_position = last_move.position;
    
    switch (last_move.direction) {
        case Direction::Up:
            next_position.y -= 1;
            break;
        case Direction::Right:
            next_position.x += 1;
            break;
        case Direction::Down:
            next_position.y += 1;
            break;
        case Direction::Left:
            next_position.x -= 1;
            break;
        default:
            break;
    }
    
    if (PositionIsInGrid(grid, next_position)) {
        i32 index = GridPositionToIndex(grid, next_position);
        char square = grid->parser->data[index];
        if (square == '#') {
            // We can't move into a '#', so we have to turn right and try again.
            last_move.direction = GetNextDirection(last_move.direction);
            result = FindNextMove(grid, last_move);
        } else {
            result.position = next_position;
            result.direction = last_move.direction;
        }
    } else {
        // We're moving off the grid, so we're done.
        result.position = next_position;
        result.direction = last_move.direction;
    }
    
    return result;
}


bool MarkedPosition(Grid* grid, Position position)
{
    bool result = false;
    
    i32 index = GridPositionToIndex(grid, position);
    if (index >= 0 && index < grid->parser->size) {
        char square = grid->parser->data[index];
        if (square != 'X') {
            grid->parser->data[index] = 'X';
            result = true;
        }
    }
    
    return result;
}


void Day06_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    Grid grid = ComputeGridDimensions(&parser);
    
    u32 part1_answer = 0;
    u32 part2_answer = 0;

    // Find starting position.
    bool found_position = false;
    Move move = {};
    
    while (!found_position && !AtEndOfFile(&parser)) {
        char current = parser.data[parser.offset];
        switch (current) {
            case '^':
                found_position = true;
                move.direction = Direction::Up;
                break;
            case '>':
                found_position = true;
                move.direction = Direction::Right;
                break;
            case 'v':
                found_position = true;
                move.direction = Direction::Down;
                break;
            case '<':
                found_position = true;
                move.direction = Direction::Left;
                break;
            default:
                break;
        }
        if (!found_position) {
            Advance(&parser);
        }
    }
    
    // Handle initial position.
    move.position = IndexToGridPosition(&grid, parser.offset);
    if (MarkedPosition(&grid, move.position)) {
        part1_answer++;
    }
    
    while (PositionIsInGrid(&grid, move.position)) {
        move = FindNextMove(&grid, move);
        if (MarkedPosition(&grid, move.position)) {
            part1_answer++;
        }
    }
    
    fprintf(stdout, "2024: Day 04 part 1: %u\n", part1_answer);
    fprintf(stdout, "2024: Day 04 part 2: %u\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
