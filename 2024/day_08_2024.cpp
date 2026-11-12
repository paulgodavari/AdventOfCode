// day_08_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <numeric>
#include <vector>


struct Antenna
{
    i64 row;
    i64 col;
};


static bool MarkAntinode(const GridView& grid, std::vector<bool>* marked, i64 row, i64 col, i64* count)
{
    if (row < 0 || row >= grid.rows || col < 0 || col >= grid.cols) {
        return false;
    }

    size_t index = (size_t) row * grid.cols + col;
    if (!(*marked)[index]) {
        (*marked)[index] = true;
        ++*count;
    }
    return true;
}


static bool SolveDay08(const File* input_file, bool harmonics, i64* answer)
{
    GridView grid = {};
    if (!ReadGrid(input_file, &grid)) {
        return false;
    }

    std::vector<Antenna> frequencies[256];
    for (i32 row = 0; row < grid.rows; ++row) {
        for (i32 col = 0; col < grid.cols; ++col) {
            u8 cell = (u8) GridCell(grid, row, col);
            if (cell != '.') {
                frequencies[cell].push_back({ row, col });
            }
        }
    }

    std::vector<bool> marked((size_t) grid.rows * grid.cols);
    i64 count = 0;
    for (const auto& antennas : frequencies) {
        for (size_t first = 0; first < antennas.size(); ++first) {
            for (size_t second = first + 1; second < antennas.size(); ++second) {
                Antenna a = antennas[first];
                Antenna b = antennas[second];
                i64 dr = b.row - a.row;
                i64 dc = b.col - a.col;
                if (!harmonics) {
                    MarkAntinode(grid, &marked, a.row - dr, a.col - dc, &count);
                    MarkAntinode(grid, &marked, b.row + dr, b.col + dc, &count);
                    continue;
                }

                i64 divisor = std::gcd(dr, dc);
                dr /= divisor;
                dc /= divisor;
                for (i64 row = a.row, col = a.col; MarkAntinode(grid, &marked, row, col, &count); row += dr, col += dc) {
                }
                for (i64 row = a.row - dr, col = a.col - dc; MarkAntinode(grid, &marked, row, col, &count); row -= dr, col -= dc) {
                }
            }
        }
    }
    *answer = count;
    return true;
}


bool Day08Part1_2024(const File* input_file, i64* answer)
{
    return SolveDay08(input_file, false, answer);
}


bool Day08Part2_2024(const File* input_file, i64* answer)
{
    return SolveDay08(input_file, true, answer);
}
