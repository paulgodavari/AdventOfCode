// day_22.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <set>
#include <vector>


// static const char* input_file_name = "day_22.test_input";  // Part 1: 5, part 2: 7
static const char* input_file_name = "day_22.input";  // Part 1: 505, part 2:


// 21745 too low

struct Position
{
    i32 x;
    i32 y;
    i32 z;
};


static bool operator==(const Position& lhs, const Position& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}


static const Position kMaxDimension = { 10, 10, 365 };


struct Brick
{
    u32 id;
    Position start;
    Position stop;
};


bool operator==(const Brick& lhs, const Brick& rhs)
{
    return (lhs.start == rhs.start) && (lhs.stop == rhs.stop);
}


bool ZCompare(const Brick& lhs, const Brick& rhs)
{
    u32 lz_min = std::min(lhs.start.z, lhs.stop.z);
    u32 rz_min = std::min(rhs.start.z, rhs.stop.z);
    return lz_min < rz_min;
}


struct Grid
{
    u32 space[kMaxDimension.x][kMaxDimension.y][kMaxDimension.z];
};


Brick PositionFallingBrick(Grid* grid, Brick brick)
{
    Brick result = brick;
    
    i32 z_drop = 0;
    bool done_falling = false;
    for (int z = brick.start.z; z > 1; --z) {
        u32 z_test = z - 1;
        u32 x_min = std::min(brick.start.x, brick.stop.x);
        u32 x_max = std::max(brick.start.x, brick.stop.x);
        for (int x = x_min; x <= x_max; ++x) {
            u32 y_min = std::min(brick.start.y, brick.stop.y);
            u32 y_max = std::max(brick.start.y, brick.stop.y);
            for (int y = y_min; y <= y_max; ++y) {
                if (grid->space[x][y][z_test]) {
                    done_falling = true;
                    break;
                }
            }
            if (done_falling) {
                break;
            }
        }
        if (done_falling) {
            break;
        } else {
            z_drop++;
        }
    }
    
    result.start.z -= z_drop;
    result.stop.z -= z_drop;
        
    return result;
}


void AddBrickToGrid(Grid* grid, Brick brick)
{
    u32 x_min = std::min(brick.start.x, brick.stop.x);
    u32 x_max = std::max(brick.start.x, brick.stop.x);
    for (int x = x_min; x <= x_max; ++x) {
        u32 y_min = std::min(brick.start.y, brick.stop.y);
        u32 y_max = std::max(brick.start.y, brick.stop.y);
        for (int y = y_min; y <= y_max; ++y) {
            u32 z_min = std::min(brick.start.z, brick.stop.z);
            u32 z_max = std::max(brick.start.z, brick.stop.z);
            for (int z = z_min; z <= z_max; ++z) {
                assert(grid->space[x][y][z] == 0 || grid->space[x][y][z] == brick.id);
                grid->space[x][y][z] = brick.id;
            }
        }
    }
}


void RemoveBrickFromGrid(Grid* grid, Brick brick)
{
    u32 x_min = std::min(brick.start.x, brick.stop.x);
    u32 x_max = std::max(brick.start.x, brick.stop.x);
    for (int x = x_min; x <= x_max; ++x) {
        u32 y_min = std::min(brick.start.y, brick.stop.y);
        u32 y_max = std::max(brick.start.y, brick.stop.y);
        for (int y = y_min; y <= y_max; ++y) {
            u32 z_min = std::min(brick.start.z, brick.stop.z);
            u32 z_max = std::max(brick.start.z, brick.stop.z);
            for (int z = z_min; z <= z_max; ++z) {
                grid->space[x][y][z] = 0;
            }
        }
    }
}


std::set<u32> FindVerticalNeighbours(Grid* grid, Brick brick)
{
    std::set<u32> result;
    
    u32 z_test = std::max(brick.start.z, brick.stop.z) + 1;

    u32 x_min = std::min(brick.start.x, brick.stop.x);
    u32 x_max = std::max(brick.start.x, brick.stop.x);
    for (int x = x_min; x <= x_max; ++x) {
        u32 y_min = std::min(brick.start.y, brick.stop.y);
        u32 y_max = std::max(brick.start.y, brick.stop.y);
        for (int y = y_min; y <= y_max; ++y) {
            if (grid->space[x][y][z_test]) {
                result.insert(grid->space[x][y][z_test]);
            }
        }
    }
    
    return result;
}


bool CanRemoveBrickFromGrid(Grid* grid, Brick brick, const std::vector<Brick>& bricks)
{
    bool result = true;
    
    std::set<u32> neighbours = FindVerticalNeighbours(grid, brick);
    for (auto it = neighbours.begin(); it != neighbours.end(); ++it) {
        u32 neighbour_index = *it - 1;
        Brick neighbour = bricks[neighbour_index];
        RemoveBrickFromGrid(grid, brick);
        Brick new_position = PositionFallingBrick(grid, neighbour);
        AddBrickToGrid(grid, brick);
        if (new_position != neighbour) {
            // This neighbour would fall without us.
            result = false;
            break;
        }
    }
    
    return result;
}


u32 ComputeTotalFall(Grid* grid, Brick brick, const std::vector<Brick>& bricks, std::set<u32>* fallen)
{
    u32 result = 0;

    std::set<u32> falling_neighbours;
    std::set<u32> neighbours = FindVerticalNeighbours(grid, brick);
    RemoveBrickFromGrid(grid, brick);

    for (auto it = neighbours.begin(); it != neighbours.end(); ++it) {
        u32 neighbour_index = *it - 1;
        Brick neighbour = bricks[neighbour_index];
        Brick new_position = PositionFallingBrick(grid, neighbour);
        if (new_position != neighbour) {
            // This neighbour would fall without us.
            falling_neighbours.insert(neighbour.id);
            fallen->insert(neighbour.id);
        }
    }

    for (auto it = falling_neighbours.begin(); it != falling_neighbours.end(); ++it) {
        u32 neighbour_index = *it - 1;
        Brick neighbour = bricks[neighbour_index];
        RemoveBrickFromGrid(grid, neighbour);
    }
    
    for (auto it = falling_neighbours.begin(); it != falling_neighbours.end(); ++it) {
        u32 neighbour_index = *it - 1;
        Brick neighbour = bricks[neighbour_index];
        result += ComputeTotalFall(grid, neighbour, bricks, fallen);
    }

    for (auto it = falling_neighbours.begin(); it != falling_neighbours.end(); ++it) {
        u32 neighbour_index = *it - 1;
        Brick neighbour = bricks[neighbour_index];
        AddBrickToGrid(grid, neighbour);
    }

    AddBrickToGrid(grid, brick);

    result += falling_neighbours.size();
    
    return result;
}


void Day22()
{
    fprintf(stdout, "Day 22\n");

    u64 run_time = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };

    std::vector<Brick> input_bricks;
    Position min = {};
    Position max = {};
    
    while (!AtEndOfFile(&parser)) {
        Brick brick = {};
        brick.id = (u32) input_bricks.size() + 1;
        
        brick.start.x = ParseNumber(&parser);
        Advance(&parser);
        brick.start.y = ParseNumber(&parser);
        Advance(&parser);
        brick.start.z = ParseNumber(&parser);

        assert(parser.data[parser.offset] == '~');
        Advance(&parser);

        brick.stop.x = ParseNumber(&parser);
        Advance(&parser);
        brick.stop.y = ParseNumber(&parser);
        Advance(&parser);
        brick.stop.z = ParseNumber(&parser);

        assert(parser.data[parser.offset] == '\n');
        Advance(&parser);

        // Find the min and max dimensions.
        min.x = std::min(min.x, brick.start.x);
        min.x = std::min(min.x, brick.stop.x);
        min.y = std::min(min.y, brick.start.y);
        min.y = std::min(min.y, brick.stop.y);
        min.z = std::min(min.z, brick.start.z);
        min.z = std::min(min.z, brick.stop.z);

        max.x = std::max(max.x, brick.start.x);
        max.x = std::max(max.x, brick.stop.x);
        max.y = std::max(max.y, brick.start.y);
        max.y = std::max(max.y, brick.stop.y);
        max.z = std::max(max.z, brick.start.z);
        max.z = std::max(max.z, brick.stop.z);

        input_bricks.push_back(brick);
    }
    
    fprintf(stdout, "Found %u bricks, min=(%u, %u, %u), max=(%u, %u, %u)\n",
            (u32) input_bricks.size(), min.x, min.y, min.z, max.x, max.y, max.z);
    fprintf(stdout, "Initialization time: %.4f\n", MillisecondsSince(run_time));
    
    // Part 1
    
    // Position the bricks at the lowest possible z value in the grid.
    u64 start_time = TimeNow();
    std::sort(input_bricks.begin(), input_bricks.end(), ZCompare);
    fprintf(stdout, "Sort time: %.4f\n", MillisecondsSince(start_time));

    start_time = TimeNow();
    Grid grid = {};
    std::vector<Brick> bricks;
    for (int i = 0; i < input_bricks.size(); ++i) {
        Brick brick = input_bricks[i];
        Brick new_brick = PositionFallingBrick(&grid, brick);
        new_brick.id = i+1;
        AddBrickToGrid(&grid, new_brick);
        bricks.push_back(new_brick);
    }
    fprintf(stdout, "Falling time: %.4f\n", MillisecondsSince(start_time));

    // For each brick, find all bricks vertically adjacent and see if those will fall.
    start_time = TimeNow();
    u32 part_1_bricks = 0;
    for (int i = 0; i < bricks.size(); ++i) {
        Brick brick = bricks[i];
        if (CanRemoveBrickFromGrid(&grid, brick, bricks)) {
            part_1_bricks++;
        }
    }
    
    fprintf(stdout, "Check time: %.4f\n", MillisecondsSince(start_time));
    fprintf(stdout, "Part 1 number of bricks: %u\n", part_1_bricks);
    
    // Part 2
    start_time = TimeNow();
    u32 part_2_bricks = 0;
    
    std::set<u32> fallen;
    for (int i = 0; i < bricks.size(); ++i) {
        Brick brick = bricks[i];
        part_2_bricks += ComputeTotalFall(&grid, brick, bricks, &fallen);
    }

    fprintf(stdout, "Part 2 number of bricks: %u (%u)\n", part_2_bricks, fallen.size());
    fprintf(stdout, "Total fall time: %.4f\n", MillisecondsSince(start_time));

    fprintf(stdout, "Run time: %.4f ms\n", MillisecondsSince(run_time));
    
    CloseFile(&input_file);
}


