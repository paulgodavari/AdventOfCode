// day_14.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <unordered_map>
#include <string.h>



static const u32 kMaxRows = 100;
static const u32 kMaxCols = 100;

static const u32 kCycles = 1000000000;


struct PlatformGrid
{
    u32 rows;
    u32 cols;
    char items[kMaxRows][kMaxCols];

    bool operator==(const PlatformGrid& rhs) const
    {
        return memcmp(items, rhs.items, kMaxCols * kMaxRows * sizeof(char)) == 0;
    }
};


struct PlatformGridHash
{
    std::size_t operator()(const PlatformGrid& grid) const
    {
        std::hash<char> hasher;
        std::size_t hash = 0;
        for (int row = 0; row < grid.rows; ++row) {
            for (int col = 0; col < grid.cols; ++col) {
                hash ^= hasher(grid.items[row][col]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
        }
        return hash;
    }
};



static void TiltGridNorth(PlatformGrid* grid)
{
    for (int col = 0; col < grid->cols; ++col) {
        i32 adjust = 0;
        for (int row = 0; row < grid->rows; ++row) {
            char c = grid->items[row][col];
            switch (c) {
                case '.': {
                    adjust -= 1;
                    break;
                }
                case 'O': {
                    if (adjust < 0) {
                        i32 new_row = row + adjust;
                        grid->items[new_row][col] = 'O';
                        grid->items[row][col] = '.';
                    }
                    break;
                }
                case '#': {
                    adjust = 0;
                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }
        }
    }
}

static void TiltGridSouth(PlatformGrid* grid)
{
    for (int col = 0; col < grid->cols; ++col) {
        i32 adjust = 0;
        for (int row = (grid->rows - 1); row >= 0; --row) {
            char c = grid->items[row][col];
            switch (c) {
                case '.': {
                    adjust += 1;
                    break;
                }
                case 'O': {
                    if (adjust > 0) {
                        i32 new_row = row + adjust;
                        grid->items[new_row][col] = 'O';
                        grid->items[row][col] = '.';
                    }
                    break;
                }
                case '#': {
                    adjust = 0;
                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }
        }
    }
}


static void TiltGridEast(PlatformGrid* grid)
{
    for (int row = 0; row < grid->rows; ++row) {
        i32 adjust = 0;
        for (int col = (grid->cols - 1); col >= 0; --col) {
            char c = grid->items[row][col];
            switch (c) {
                case '.': {
                    adjust += 1;
                    break;
                }
                case 'O': {
                    if (adjust > 0) {
                        i32 new_col = col + adjust;
                        grid->items[row][new_col] = 'O';
                        grid->items[row][col] = '.';
                    }
                    break;
                }
                case '#': {
                    adjust = 0;
                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }
        }
    }
}


static void TiltGridWest(PlatformGrid* grid)
{
    for (int row = 0; row < grid->rows; ++row) {
        i32 adjust = 0;
        for (int col = 0; col < grid->cols; ++col) {
            char c = grid->items[row][col];
            switch (c) {
                case '.': {
                    adjust -= 1;
                    break;
                }
                case 'O': {
                    if (adjust < 0) {
                        i32 new_col = col + adjust;
                        grid->items[row][new_col] = 'O';
                        grid->items[row][col] = '.';
                    }
                    break;
                }
                case '#': {
                    adjust = 0;
                    break;
                }
                default: {
                    assert(0);
                    break;
                }
            }
        }
    }
}


static u32 ScoreGrid(PlatformGrid* grid)
{
    u32 result = 0;

    for (int row = 0; row < grid->rows; ++row) {
        u32 row_count = 0;
        for (int col = 0; col < grid->cols; ++col) {
            if (grid->items[row][col] == 'O') {
                row_count++;
            }
        }
        result += row_count * (grid->rows - row);
    }

    return result;
}

static bool ReadPlatform(const File* input_file, PlatformGrid* grid)
{
    String input = { input_file->data, input_file->size };
    while (input.size) {
        String row = ReadLine(&input);
        if (!row.size || row.size > kMaxCols || grid->rows == kMaxRows || (grid->rows && row.size != grid->cols)) {
            return false;
        }
        grid->cols = (u32) row.size;
        for (u32 col = 0; col < grid->cols; ++col) {
            char cell = row.start[col];
            if (cell != '.' && cell != '#' && cell != 'O') {
                return false;
            }
            grid->items[grid->rows][col] = cell;
        }
        ++grid->rows;
    }
    return grid->rows != 0;
}


bool Day14Part1(const File* input_file, i64* answer)
{
    PlatformGrid grid = {};
    if (!ReadPlatform(input_file, &grid)) {
        return false;
    }

    TiltGridNorth(&grid);
    *answer = ScoreGrid(&grid);
    return true;
}


bool Day14Part2(const File* input_file, i64* answer)
{
    PlatformGrid grid = {};
    if (!ReadPlatform(input_file, &grid)) {
        return false;
    }

    std::unordered_map<PlatformGrid, u32, PlatformGridHash> seen;
    seen.emplace(grid, 0);
    u32 cycle = 0;
    while (cycle < kCycles) {
        TiltGridNorth(&grid);
        TiltGridWest(&grid);
        TiltGridSouth(&grid);
        TiltGridEast(&grid);
        ++cycle;

        auto entry = seen.emplace(grid, cycle);
        if (!entry.second) {
            u32 period = cycle - entry.first->second;
            cycle += (kCycles - cycle) / period * period;
        }
    }

    *answer = ScoreGrid(&grid);
    return true;
}
