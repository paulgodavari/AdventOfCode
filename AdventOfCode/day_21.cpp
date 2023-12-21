// day_21.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <set>
#include <vector>


// static const char* input_file_name = "day_21.test_input";  // Part 1: 16, part 2:
// static const u32 kMaxSteps = 6;

static const char* input_file_name = "day_21.input";  // Part 1: , part 2:
static const u32 kMaxSteps = 64;

static const u32 kMaxRows = 131;
static const u32 kMaxCols = 131;


enum Terrain
{
    kTerrainEmpty = 0,
    kTerrainRock,
    kTerrainStep
};


struct Position
{
    i32 row;
    i32 col;
};


bool operator<(const Position& lhs, const Position& rhs)
{
    bool result = false;
    if (lhs.row < rhs.row) {
        result = true;
    } else {
        if (lhs.row == rhs.row) {
            result = lhs.col < rhs.col;
        }
    }
    return result;
}


struct Item
{
    Terrain terrain;
    bool visited;
};


struct Grid
{
    Item items[kMaxRows][kMaxCols];
};


void PrintGridPositions(const Grid& grid, const std::vector<Position> positions, Position dimensions)
{
    Grid g = grid;
    
    for (int i = 0 ; i < positions.size(); ++i) {
        Position p = positions[i];
        Terrain t = g.items[p.row][p.col].terrain;
        if (t == kTerrainEmpty || t == kTerrainStep) {
            g.items[p.row][p.col].terrain = kTerrainStep;
        } else {
            assert(0);
        }
    }
    
    for (int row = 0; row < dimensions.row; ++row) {
        for (int col = 0; col < (dimensions.col - 1); ++col) {
            char c = '.';
            if (g.items[row][col].terrain == kTerrainRock) {
                c = '#';
            } else if (g.items[row][col].terrain == kTerrainStep) {
                c = 'O';
            }
            fprintf(stdout, "%c", c);
        }
        fprintf(stdout, "\n");
    }
}


void Day21()
{
    fprintf(stdout, "Day 21\n");

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
    
    // Find grid size and starting position.

    Position dimensions = {};
    Position start_pos = {};
    Position current_pos = {};
    Grid grid = {};

    while (!AtEndOfFile(&parser)) {
        current_pos.col = 0;
        while (!AtEndOfLine(&parser)) {
            char current = parser.data[parser.offset];
            switch (current) {
                case 'S':
                    start_pos = current_pos;
                    // Fall through.
                case '.': {
                    grid.items[current_pos.row][current_pos.col].terrain = kTerrainEmpty;
                    break;
                }
                case '#': {
                    grid.items[current_pos.row][current_pos.col].terrain = kTerrainRock;
                    break;
                }
                default:
                    assert(0);
                    break;
            }
            if (current == 'S') {
                
            }
            Advance(&parser);
            current_pos.col++;
        }
        Advance(&parser);
        current_pos.col++;
        current_pos.row++;
        if (dimensions.col < current_pos.col) {
            dimensions.col = current_pos.col;
        }
    }
    
    dimensions.row = current_pos.row;
    fprintf(stdout, "Start position: r: %u, c: %u\n", start_pos.row, start_pos.col);
    fprintf(stdout, "Dimensions: r: %u, c: %u\n", dimensions.row, dimensions.col);
    fprintf(stdout, "Input processing time: %.4f\n", MillisecondsSince(start_time));
    
    start_time = TimeNow();
    
    u32 possible_positions = 0;
    u32 steps = 0;
    std::vector<Position> current_positions;
    current_positions.push_back(start_pos);
    
    while (steps < kMaxSteps) {
        std::set<Position> next_positions;
        for (int i = 0; i < current_positions.size(); ++i) {
            Position p = current_positions[i];
            // Check all neighbors
            Position up = { p.row - 1, p.col };
            if ((up.row >= 0) && (grid.items[up.row][up.col].terrain == kTerrainEmpty)) {
                next_positions.insert(up);
            }
            Position down = { p.row + 1, p.col };
            if ((down.row < dimensions.row) && (grid.items[down.row][down.col].terrain == kTerrainEmpty)) {
                next_positions.insert(down);
            }
            Position left = { p.row, p.col - 1 };
            if ((left.col >= 0) && (grid.items[left.row][left.col].terrain == kTerrainEmpty)) {
                next_positions.insert(left);
            }
            Position right = { p.row, p.col + 1 };
            if ((right.col < (dimensions.col - 1)) && (grid.items[right.row][right.col].terrain == kTerrainEmpty)) {
                next_positions.insert(right);
            }
        }
        
        current_positions.clear();
        for (auto it = next_positions.begin(); it != next_positions.end(); ++it) {
            current_positions.push_back(*it);
        }
        
        possible_positions = current_positions.size();
        steps++;
    
        // fprintf(stdout, "Step %u\n", steps);
        // PrintGridPositions(grid, current_positions, dimensions);
        // fprintf(stdout, "\n\n", steps);
    }

    fprintf(stdout, "Path time: %.4f ms\n", MillisecondsSince(start_time));
    fprintf(stdout, "Position count: %u\n", possible_positions);

    fprintf(stdout, "Run time: %.4f ms\n", MillisecondsSince(run_time));
    
    CloseFile(&input_file);
}


