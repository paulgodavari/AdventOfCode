// day_04_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"


bool Day04Part1_2024(const File* input_file, i64* answer)
{
    GridView grid = {};
    if (!ReadGrid(input_file, &grid)) {
        return false;
    }

    i64 count = 0;
    for (i64 row = 0; row < grid.rows; ++row) {
        for (i64 col = 0; col < grid.cols; ++col) {
            if (GridCell(grid, row, col) != 'X') {
                continue;
            }
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    count += GridCell(grid, row + dr, col + dc) == 'M' &&
                             GridCell(grid, row + 2 * dr, col + 2 * dc) == 'A' &&
                             GridCell(grid, row + 3 * dr, col + 3 * dc) == 'S';
                }
            }
        }
    }
    *answer = count;
    return true;
}


static bool IsMASPair(char first, char second)
{
    return (first == 'M' && second == 'S') || (first == 'S' && second == 'M');
}


bool Day04Part2_2024(const File* input_file, i64* answer)
{
    GridView grid = {};
    if (!ReadGrid(input_file, &grid)) {
        return false;
    }

    i64 count = 0;
    for (i64 row = 1; row + 1 < grid.rows; ++row) {
        for (i64 col = 1; col + 1 < grid.cols; ++col) {
            count += GridCell(grid, row, col) == 'A' &&
                     IsMASPair(GridCell(grid, row - 1, col - 1), GridCell(grid, row + 1, col + 1)) &&
                     IsMASPair(GridCell(grid, row - 1, col + 1), GridCell(grid, row + 1, col - 1));
        }
    }
    *answer = count;
    return true;
}
