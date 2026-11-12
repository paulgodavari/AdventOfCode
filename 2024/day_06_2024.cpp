// day_06_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


static const int kDirectionCount = 4;
static const int kRowStep[kDirectionCount] = { -1, 0, 1, 0 };
static const int kColStep[kDirectionCount] = { 0, 1, 0, -1 };


static size_t TurnRight(size_t state)
{
    size_t cell = state / kDirectionCount;
    size_t direction = state % kDirectionCount;
    return cell * kDirectionCount + (direction + 1) % kDirectionCount;
}


static bool SolveDay06(const File* input_file, bool obstructions, i64* answer)
{
    GridView grid = {};
    if (!ReadGrid(input_file, &grid)) {
        return false;
    }

    size_t cells = (size_t) grid.rows * grid.cols;
    if (cells > SIZE_MAX / kDirectionCount) {
        return false;
    }

    size_t start = SIZE_MAX;
    std::vector<size_t> moves(cells * kDirectionCount, SIZE_MAX);
    for (i32 row = 0; row < grid.rows; ++row) {
        for (i32 col = 0; col < grid.cols; ++col) {
            char tile = GridCell(grid, row, col);
            size_t cell = (size_t) row * grid.cols + col;
            if (tile == '^') {
                if (start != SIZE_MAX) {
                    return false;
                }
                start = cell * kDirectionCount;
            } else if (tile != '.' && tile != '#') {
                return false;
            }

            for (int direction = 0; direction < kDirectionCount; ++direction) {
                size_t state = cell * kDirectionCount + direction;
                i64 next_row = (i64) row + kRowStep[direction];
                i64 next_col = (i64) col + kColStep[direction];
                char next_tile = GridCell(grid, next_row, next_col);
                if (next_tile == '#') {
                    moves[state] = TurnRight(state);
                } else if (next_tile) {
                    size_t next_cell = (size_t) next_row * grid.cols + next_col;
                    moves[state] = next_cell * kDirectionCount + direction;
                }
            }
        }
    }
    if (start == SIZE_MAX) {
        return false;
    }

    std::vector<bool> visited(moves.size());
    std::vector<bool> route(cells);
    i64 count = 0;
    for (size_t state = start; state != SIZE_MAX; state = moves[state]) {
        if (visited[state]) {
            return false;
        }
        visited[state] = true;
        size_t cell = state / kDirectionCount;
        if (!route[cell]) {
            route[cell] = true;
            ++count;
        }
    }
    if (!obstructions) {
        *answer = count;
        return true;
    }

    // An obstruction can change the route when the guard first tries to enter its cell.
    std::vector<size_t> seen(moves.size());
    i64 loops = 0;
    for (size_t cell = 0; cell < cells; ++cell) {
        if (!route[cell] || cell == start / kDirectionCount) {
            continue;
        }

        size_t state = start;
        while (state != SIZE_MAX && seen[state] != cell + 1) {
            seen[state] = cell + 1;
            size_t next = moves[state];
            state = next != SIZE_MAX && next / kDirectionCount == cell ? TurnRight(state) : next;
        }
        loops += state != SIZE_MAX;
    }
    *answer = loops;
    return true;
}


bool Day06Part1_2024(const File* input_file, i64* answer)
{
    return SolveDay06(input_file, false, answer);
}


bool Day06Part2_2024(const File* input_file, i64* answer)
{
    return SolveDay06(input_file, true, answer);
}
