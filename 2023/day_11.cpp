// day_11.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


struct Galaxy
{
    i64 row;
    i64 col;
};


static bool SolveDay11(const File* input_file, i64 expansion, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<Galaxy> galaxies;
    std::vector<bool> occupied_columns;
    i64 row = 0;
    while (input.size) {
        String line = ReadLine(&input);
        if (row == 0) {
            occupied_columns.resize(line.size);
        }
        if (!line.size || line.size != occupied_columns.size()) {
            return false;
        }

        bool occupied = false;
        for (size_t col = 0; col < line.size; ++col) {
            if (line.start[col] == '#') {
                galaxies.push_back({ row, (i64) col });
                occupied_columns[col] = true;
                occupied = true;
            } else if (line.start[col] != '.') {
                return false;
            }
        }
        if (__builtin_add_overflow(row, occupied ? 1 : expansion, &row)) {
            return false;
        }
    }

    std::vector<i64> column_positions(occupied_columns.size());
    i64 position = 0;
    for (size_t col = 0; col < occupied_columns.size(); ++col) {
        column_positions[col] = position;
        if (__builtin_add_overflow(position, occupied_columns[col] ? 1 : expansion, &position)) {
            return false;
        }
    }
    for (Galaxy& galaxy : galaxies) {
        galaxy.col = column_positions[galaxy.col];
    }

    i64 total = 0;
    for (size_t first = 0; first < galaxies.size(); ++first) {
        for (size_t second = first + 1; second < galaxies.size(); ++second) {
            i64 row_distance = galaxies[second].row - galaxies[first].row;
            i64 col_distance = llabs(galaxies[second].col - galaxies[first].col);
            if (__builtin_add_overflow(total, row_distance, &total) || __builtin_add_overflow(total, col_distance, &total)) {
                return false;
            }
        }
    }

    *answer = total;
    return true;
}


bool Day11Part1(const File* input_file, i64* answer)
{
    return SolveDay11(input_file, 2, answer);
}


bool Day11Part2(const File* input_file, i64* answer)
{
    return SolveDay11(input_file, 1000000, answer);
}


bool Day11Part2Test(const File* input_file, i64* answer)
{
    return SolveDay11(input_file, 10, answer);
}
