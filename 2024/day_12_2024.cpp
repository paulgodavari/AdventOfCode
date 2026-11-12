// day_12_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.

#include "util/days.h"

#include <vector>


struct GardenPosition
{
    i32 row;
    i32 col;

    bool operator==(const GardenPosition& other) const
    {
        return row == other.row && col == other.col;
    }
};


static bool IsValidPosition(const GridView* grid, GardenPosition position)
{
    return position.row >= 0 && position.row < grid->rows && position.col >= 0 && position.col < grid->cols;
}


static i64 FindRegion(const GridView* grid, GardenPosition start, std::vector<bool>* visited, std::vector<GardenPosition>* region)
{
    const GardenPosition kSteps[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
    char plant = GridCell(*grid, start.row, start.col);
    (*visited)[(size_t) start.row * grid->cols + start.col] = true;
    region->clear();
    region->push_back(start);

    i64 perimeter = 0;
    for (size_t index = 0; index < region->size(); ++index) {
        GardenPosition position = (*region)[index];
        for (GardenPosition step : kSteps) {
            GardenPosition next = { position.row + step.row, position.col + step.col };
            if (!IsValidPosition(grid, next) || GridCell(*grid, next.row, next.col) != plant) {
                ++perimeter;
                continue;
            }

            size_t cell = (size_t) next.row * grid->cols + next.col;
            if (!(*visited)[cell]) {
                (*visited)[cell] = true;
                region->push_back(next);
            }
        }
    }
    return perimeter;
}


bool Day12Part1_2024(const File* input_file, i64* answer)
{
    GridView grid = {};
    if (!ReadGrid(input_file, &grid)) {
        return false;
    }

    std::vector<bool> visited((size_t) grid.rows * grid.cols);
    std::vector<GardenPosition> region;
    i64 total = 0;
    for (i32 row = 0; row < grid.rows; ++row) {
        for (i32 col = 0; col < grid.cols; ++col) {
            if (!visited[(size_t) row * grid.cols + col]) {
                i64 perimeter = FindRegion(&grid, { row, col }, &visited, &region);
                i64 price = 0;
                if (__builtin_mul_overflow(perimeter, region.size(), &price) || __builtin_add_overflow(total, price, &total)) {
                    return false;
                }
            }
        }
    }
    *answer = total;
    return true;
}


// Part 2 border walk in progress.
enum class GardenDirection
{
    Invalid = 0,
    Up,
    Right,
    Down,
    Left
};


struct GardenMove
{
    GardenPosition position;
    GardenDirection direction;

    bool operator==(const GardenMove& other) const
    {
        return position == other.position && direction == other.direction;
    }
};


static bool HasBorderForDirection(const GridView* grid, GardenPosition position, GardenDirection direction)
{
    bool result = false;

    GardenPosition new_position = position;
    switch (direction) {
        case GardenDirection::Up:
            new_position.row -= 1;
            break;
        case GardenDirection::Right:
            new_position.col += 1;
            break;
        case GardenDirection::Down:
            new_position.row += 1;
            break;
        case GardenDirection::Left:
            new_position.col -= 1;
            break;
        default:
            assert(0);
            break;
    }

    if (IsValidPosition(grid, new_position)) {
        char current = GridCell(*grid, position.row, position.col);
        char next = GridCell(*grid, new_position.row, new_position.col);
        if (current != next) {
            result = true;
        }
    } else {
        result = true;
    }

    return result;
}


static GardenMove GetNextMove(const GridView* grid, GardenMove previous_move)
{
    GardenMove result = {};

    switch (previous_move.direction) {
        case GardenDirection::Up: {
            // See if we can keep going up.
            GardenPosition test = { previous_move.position.row - 1, previous_move.position.col };
            if (!IsValidPosition(grid, test)) {
                result.position = previous_move.position;
                result.direction = GardenDirection::Right;
            }
            break;
        }
        default:
            break;
    }

    return result;
}


[[maybe_unused]] static u64 ComputeSides(const GridView* grid, const std::vector<GardenPosition>& positions)
{
    u64 result = 0;

    // Find the first position with a border.
    GardenMove start = { { -1, -1 }, GardenDirection::Invalid };

    for (GardenPosition position : positions) {
        if (HasBorderForDirection(grid, position, GardenDirection::Up)) {
            start.position = position;
            start.direction = GardenDirection::Right;
            break;
        }
        if (HasBorderForDirection(grid, position, GardenDirection::Right)) {
            start.position = position;
            start.direction = GardenDirection::Down;
            break;
        }
        if (HasBorderForDirection(grid, position, GardenDirection::Down)) {
            start.position = position;
            start.direction = GardenDirection::Left;
            break;
        }
        if (HasBorderForDirection(grid, position, GardenDirection::Left)) {
            start.position = position;
            start.direction = GardenDirection::Up;
            break;
        }
    }

    assert(IsValidPosition(grid, start.position));
    assert(start.direction != GardenDirection::Invalid);

    GardenMove current_move = start;
    GardenMove next_move = GetNextMove(grid, start);

    while (!(next_move == start)) {
        if (next_move.direction != current_move.direction) {
            // Each change in direction is a new corner, number of corners == number of sides.
            result++;
        }
        current_move = next_move;
        next_move = GetNextMove(grid, current_move);
    }

    return result;
}
