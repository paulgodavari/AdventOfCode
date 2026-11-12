// day_22.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <algorithm>
#include <set>
#include <vector>





namespace {

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
    size_t height;
    size_t depth;
    std::vector<u32> space;

    u32& At(int x, int y, int z)
    {
        return space[((size_t) x * height + y) * depth + z];
    }
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
                if (grid->At(x, y, z_test)) {
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
                assert(grid->At(x, y, z) == 0 || grid->At(x, y, z) == brick.id);
                grid->At(x, y, z) = brick.id;
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
                grid->At(x, y, z) = 0;
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
            if (grid->At(x, y, z_test)) {
                result.insert(grid->At(x, y, z_test));
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


[[maybe_unused]] u32 ComputeTotalFall(Grid* grid, Brick brick, const std::vector<Brick>& bricks, std::set<u32>* fallen)
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

bool ReadPosition(String* line, Position* position)
{
    i32* values[] = { &position->x, &position->y, &position->z };
    for (int index = 0; index < 3; ++index) {
        i64 value = 0;
        if (!ReadInteger(line, &value) || value < 0 || value >= INT32_MAX - 1) {
            return false;
        }
        *values[index] = (i32) value;
        if (index < 2) {
            if (!line->size || *line->start != ',') {
                return false;
            }
            ++line->start;
            --line->size;
        }
    }
    return position->z > 0;
}

} // namespace


bool Day22Part1(const File* input_file, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<Brick> bricks;
    Position maximum = {};
    while (input.size) {
        String line = ReadLine(&input);
        Position first = {};
        Position second = {};
        if (!ReadPosition(&line, &first) || !line.size || *line.start != '~') {
            return false;
        }
        ++line.start;
        --line.size;
        if (!ReadPosition(&line, &second) || line.size || bricks.size() >= INT32_MAX) {
            return false;
        }

        Position start = { std::min(first.x, second.x), std::min(first.y, second.y), std::min(first.z, second.z) };
        Position stop = { std::max(first.x, second.x), std::max(first.y, second.y), std::max(first.z, second.z) };
        bricks.push_back({ 0, start, stop });
        maximum.x = std::max(maximum.x, stop.x);
        maximum.y = std::max(maximum.y, stop.y);
        maximum.z = std::max(maximum.z, stop.z);
    }

    Grid grid = { (size_t) maximum.y + 1, (size_t) maximum.z + 2, {} };
    size_t cells = (size_t) maximum.x + 1;
    if (__builtin_mul_overflow(cells, grid.height, &cells) || __builtin_mul_overflow(cells, grid.depth, &cells) ||
        cells > grid.space.max_size()) {
        return false;
    }
    grid.space.resize(cells);

    std::sort(bricks.begin(), bricks.end(), ZCompare);
    for (size_t index = 0; index < bricks.size(); ++index) {
        Brick brick = PositionFallingBrick(&grid, bricks[index]);
        brick.id = (u32) index + 1;
        AddBrickToGrid(&grid, brick);
        bricks[index] = brick;
    }

    i64 removable = 0;
    for (Brick brick : bricks) {
        removable += CanRemoveBrickFromGrid(&grid, brick, bricks);
    }
    *answer = removable;
    return true;
}
