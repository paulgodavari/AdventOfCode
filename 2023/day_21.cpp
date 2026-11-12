// day_21.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


static bool SolveDay21(const File* input_file, int steps, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<String> rows;
    size_t start = SIZE_MAX;
    while (input.size) {
        String row = ReadLine(&input);
        if (!row.size || (!rows.empty() && row.size != rows[0].size)) {
            return false;
        }
        for (size_t col = 0; col < row.size; ++col) {
            char cell = row.start[col];
            if (cell == 'S') {
                if (start != SIZE_MAX) {
                    return false;
                }
                start = rows.size() * row.size + col;
            } else if (cell != '.' && cell != '#') {
                return false;
            }
        }
        rows.push_back(row);
    }
    if (start == SIZE_MAX) {
        return false;
    }

    size_t width = rows[0].size;
    std::vector<u8> current(width * rows.size());
    std::vector<u8> next(current.size());
    current[start] = 1;
    for (int step = 0; step < steps; ++step) {
        for (size_t row = 0; row < rows.size(); ++row) {
            for (size_t col = 0; col < width; ++col) {
                size_t index = row * width + col;
                next[index] = rows[row].start[col] != '#' &&
                              ((row && current[index - width]) ||
                               (row + 1 < rows.size() && current[index + width]) ||
                               (col && current[index - 1]) ||
                               (col + 1 < width && current[index + 1]));
            }
        }
        current.swap(next);
    }

    i64 count = 0;
    for (u8 reachable : current) {
        count += reachable;
    }
    *answer = count;
    return true;
}


bool Day21Part1(const File* input_file, i64* answer)
{
    return SolveDay21(input_file, 64, answer);
}


bool Day21Part1Test(const File* input_file, i64* answer)
{
    return SolveDay21(input_file, 6, answer);
}
