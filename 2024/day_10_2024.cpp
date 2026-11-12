// day_10_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


static i64 CountTrails(const GridView& grid, i32 row, i32 col, char height, size_t* peak_visits, size_t trailhead)
{
    if (height == '9') {
        if (peak_visits) {
            size_t peak = (size_t) row * grid.cols + col;
            if (peak_visits[peak] == trailhead) {
                return 0;
            }
            peak_visits[peak] = trailhead;
        }
        return 1;
    }

    const i32 kRowStep[] = { -1, 0, 1, 0 };
    const i32 kColStep[] = { 0, 1, 0, -1 };
    i64 count = 0;
    for (int direction = 0; direction < 4; ++direction) {
        i32 next_row = row + kRowStep[direction];
        i32 next_col = col + kColStep[direction];
        if (GridCell(grid, next_row, next_col) == height + 1) {
            count += CountTrails(grid, next_row, next_col, height + 1, peak_visits, trailhead);
        }
    }
    return count;
}


static bool SolveDay10(const File* input_file, bool count_paths, i64* answer)
{
    GridView grid = {};
    if (!ReadGrid(input_file, &grid)) {
        return false;
    }

    std::vector<size_t> peak_visits;
    if (!count_paths) {
        peak_visits.resize((size_t) grid.rows * grid.cols);
    }

    i64 total = 0;
    for (i32 row = 0; row < grid.rows; ++row) {
        for (i32 col = 0; col < grid.cols; ++col) {
            if (GridCell(grid, row, col) == '0') {
                size_t trailhead = (size_t) row * grid.cols + col + 1;
                i64 count = CountTrails(grid, row, col, '0', count_paths ? nullptr : peak_visits.data(), trailhead);
                if (__builtin_add_overflow(total, count, &total)) {
                    return false;
                }
            }
        }
    }
    *answer = total;
    return true;
}


bool Day10Part1_2024(const File* input_file, i64* answer)
{
    return SolveDay10(input_file, false, answer);
}


bool Day10Part2_2024(const File* input_file, i64* answer)
{
    return SolveDay10(input_file, true, answer);
}
