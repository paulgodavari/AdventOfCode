// day_18.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <vector>


struct Position
{
    i32 row;
    i32 col;
};


// static const char* input_file_name = "day_18.test_input";  // Part 1: 62, part 2:
// static Position start_pos = {};

static const char* input_file_name = "day_18.input";  // Part 1: 46359, part 2:
static Position start_pos = { 275, 41 };  // Based on having computing the input's min/max dimensions


static const u32 kMaxRows = 400;
static const u32 kMaxCols = 400;


struct Color
{
    u32 r;
    u32 g;
    u32 b;
};


enum Direction
{
    kDirectionInvalid = 0,
    kDirectionUp,
    kDirectionDown,
    kDirectionLeft,
    kDirectionRight
};


enum Terrain
{
    kTerrainEmpty = 0,
    kTerrainDash,
    kTerrainPipe,
    kTerrainF,
    kTerrain7,
    kTerrainL,
    kTerrainJ
};


struct Instruction
{
    Direction dir;
    u32 value;
    Color edge;
};


struct Grid
{
    Terrain state[kMaxRows][kMaxCols];
};


struct Dimensions
{
    Position min;
    Position max;
};


Terrain TerrainFromDirections(Direction first, Direction second)
{
    Terrain result = kTerrainEmpty;

    bool first_vertical = (first == kDirectionUp) || (first == kDirectionDown);
    bool second_vertical = (second == kDirectionUp) || (second == kDirectionDown);
    if (first_vertical == second_vertical) {
        result = first_vertical ? kTerrainPipe : kTerrainDash;
        return result;
    }
    
    switch (first) {
        case kDirectionUp:
            result = (second == kDirectionLeft) ? kTerrain7 : kTerrainF;
            break;
        case kDirectionDown:
            result = (second == kDirectionLeft) ? kTerrainJ : kTerrainL;
            break;
        case kDirectionLeft:
            result = (second == kDirectionUp) ? kTerrainL : kTerrainF;
            break;
        case kDirectionRight:
            result = (second == kDirectionUp) ? kTerrainJ : kTerrain7;
            break;
        default:
            assert(0);
            break;
    }
    
    return result;
}


void PrintGrid(const Grid& grid)
{
    for (int row = 0; row < kMaxRows; ++row) {
        for (int col = 0; col < kMaxCols; ++col) {
            char c = 'X';  // Some obviously visible bad character
            switch (grid.state[row][col]) {
                case kTerrainEmpty:
                    c = '.';
                    break;
                case kTerrainDash:
                    c = '-';
                    break;
                case kTerrainPipe:
                    c = '|';
                    break;
                case kTerrainF:
                    c = 'F';
                    break;
                case kTerrain7:
                    c = '7';
                    break;
                case kTerrainL:
                    c = 'L';
                    break;
                case kTerrainJ:
                    c = 'J';
                    break;
            }
            fprintf(stdout, "%c", c);
        }
        fprintf(stdout, "\n");
    }
}


Dimensions FindGridDimensions(const std::vector<Instruction>& instructions)
{
    Dimensions result = {};
    
    Position pos = {};
    
    for (int i = 0; i < instructions.size(); ++i) {
        Instruction inst = instructions[i];
        switch (inst.dir) {
            case kDirectionUp:
                pos.row -= inst.value;
                break;
            case kDirectionDown:
                pos.row += inst.value;
                break;
            case kDirectionLeft:
                pos.col -= inst.value;
                break;
            case kDirectionRight:
                pos.col += inst.value;
                break;
            default:
                assert(0);
                break;
        }
        
        if (pos.row < result.min.row) {
            result.min.row = pos.row;
        }
        if (pos.col < result.min.col) {
            result.min.col = pos.col;
        }
        if (pos.row > result.max.row) {
            result.max.row = pos.row;
        }
        if (pos.col > result.max.col) {
            result.max.col = pos.col;
        }
    }
    
    return result;
}


void Day18()
{
    fprintf(stdout, "Day 18\n");

    u64 run_time = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    fprintf(stdout, "Initialization time: %.4f\n", MillisecondsSince(run_time));
    
    // Part 1
    u64 start_time = TimeNow();

    std::vector<Instruction> instructions;
    while (!AtEndOfFile(&parser)) {
        char dir;
        u32 value;
        u32 r, g, b;
        sscanf(&parser.data[parser.offset], "%c %u (#%2x%2x%2x)\n", &dir, &value, &r, &g, &b);
        Direction direction = kDirectionInvalid;
        switch (dir) {
            case 'U':
                direction = kDirectionUp;
                break;
            case 'D':
                direction = kDirectionDown;
                break;
            case 'L':
                direction = kDirectionLeft;
                break;
            case 'R':
                direction = kDirectionRight;
                break;
            default:
                break;
        }
        Instruction inst = { direction, value, { r, g, b} };
        instructions.push_back(inst);
        while (!AtEndOfLine(&parser)) {
            Advance(&parser);
        }
        Advance(&parser);
    }
    
    fprintf(stdout, "Parse time: %.4f ms\n", MillisecondsSince(start_time));
    start_time = TimeNow();

    Dimensions dim = FindGridDimensions(instructions);
    fprintf(stdout, "Part 1 grid dimensions\n\tMin: (r: %d, c: %d)\n\tMax: (r: %d, c: %d) found in %.4f ms\n",
            dim.min.row, dim.min.col, dim.max.row, dim.max.col, MillisecondsSince(start_time));
    start_time = TimeNow();
    
    Position pos = start_pos;
    Grid grid = {};
    Direction last_dir = kDirectionInvalid;
    Direction first_dir = kDirectionInvalid;

    for (int i = 0; i < instructions.size(); ++i) {
        Instruction inst = instructions[i];
        Position next_pos = pos;
        switch (inst.dir) {
            case kDirectionUp:
                next_pos.row -= inst.value;
                for (int row = next_pos.row; row <= pos.row; ++row) {
                    grid.state[row][pos.col] = kTerrainPipe;
                }
                break;
            case kDirectionDown:
                next_pos.row += inst.value;
                for (int row = pos.row; row <= next_pos.row; ++row) {
                    grid.state[row][pos.col] = kTerrainPipe;
                }
                break;
            case kDirectionLeft:
                next_pos.col -= inst.value;
                for (int col = next_pos.col; col <= pos.col; ++col) {
                    grid.state[pos.row][col] = kTerrainDash;
                }
                break;
            case kDirectionRight:
                next_pos.col += inst.value;
                for (int col = pos.col; col <= next_pos.col; ++col) {
                    grid.state[pos.row][col] = kTerrainDash;
                }
                break;
            default:
                assert(0);
                break;
        }
        
        if (first_dir == kDirectionInvalid) {
            first_dir = inst.dir;
        } else {
            grid.state[pos.row][pos.col] = TerrainFromDirections(last_dir, inst.dir);
        }
        pos = next_pos;
        last_dir = inst.dir;
    }
    
    grid.state[start_pos.row][start_pos.col] = TerrainFromDirections(last_dir, first_dir);
    
    // PrintGrid(grid);
    
    fprintf(stdout, "Process instructions time: %.4f ms\n", MillisecondsSince(start_time));
    start_time = TimeNow();

    u32 part_1_sum = 0;
    for (int row = 0; row < kMaxRows; ++row) {
        bool should_include = false;
        for (int col = 0; col < kMaxCols; ++col) {
            Terrain terrain = grid.state[row][col];
            if (terrain == kTerrainPipe || terrain == kTerrainL || terrain == kTerrainJ) {
                should_include = !should_include;
            }
            if (terrain == kTerrainEmpty) {
                if (should_include) {
                    part_1_sum++;
                }
            } else {
                part_1_sum++;
            }
        }
    }
    
    fprintf(stdout, "Part 1 sum: %u (%.4f ms)\n", part_1_sum, MillisecondsSince(start_time));
    
    // Part 2
    start_time = TimeNow();
    
    parser.offset = 0;
    instructions.clear();
    while (!AtEndOfFile(&parser)) {
        char old_dir;
        u32 old_value;
        u32 dir, value;
        sscanf(&parser.data[parser.offset], "%c %u (#%5x%x)\n", &old_dir, &old_value, &value, &dir);
        Direction direction = kDirectionInvalid;
        switch (dir) {
            case 3:
                direction = kDirectionUp;
                break;
            case 1:
                direction = kDirectionDown;
                break;
            case 2:
                direction = kDirectionLeft;
                break;
            case 0:
                direction = kDirectionRight;
                break;
            default:
                break;
        }
        Instruction inst = { direction, value };
        instructions.push_back(inst);
        while (!AtEndOfLine(&parser)) {
            Advance(&parser);
        }
        Advance(&parser);
    }
 
    dim = FindGridDimensions(instructions);
    fprintf(stdout, "Part 2 grid dimensions\n\tMin: (r: %d, c: %d)\n\tMax: (r: %d, c: %d) found in %.4f ms\n",
            dim.min.row, dim.min.col, dim.max.row, dim.max.col, MillisecondsSince(start_time));

    fprintf(stdout, "Run time: %.4f ms\n", MillisecondsSince(run_time));
    
    CloseFile(&input_file);
}


