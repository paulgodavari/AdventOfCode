// day_03.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


struct EngineGear
{
    i64 first;
    i64 second;
    int count;
};


static bool IsDigit(char character)
{
    return character >= '0' && character <= '9';
}


static bool SolveDay03(const File* input_file, bool gear_ratios, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<String> rows;
    while (input.size) {
        String row = ReadLine(&input);
        if (!row.size || (!rows.empty() && row.size != rows[0].size)) {
            return false;
        }
        rows.push_back(row);
    }
    if (rows.empty()) {
        return false;
    }

    size_t width = rows[0].size;
    std::vector<EngineGear> gears;
    if (gear_ratios) {
        gears.resize(width * rows.size());
    }

    i64 total = 0;
    for (size_t row = 0; row < rows.size(); ++row) {
        for (size_t col = 0; col < width;) {
            if (!IsDigit(rows[row].start[col])) {
                ++col;
                continue;
            }

            size_t first_col = col;
            i64 number = 0;
            do {
                i64 digit = rows[row].start[col++] - '0';
                if (number > (INT64_MAX - digit) / 10) {
                    return false;
                }
                number = number * 10 + digit;
            } while (col < width && IsDigit(rows[row].start[col]));

            size_t top = row ? row - 1 : 0;
            size_t left = first_col ? first_col - 1 : 0;
            bool adjacent = false;
            for (size_t r = top; r <= row + 1 && r < rows.size(); ++r) {
                for (size_t c = left; c <= col && c < width; ++c) {
                    char cell = rows[r].start[c];
                    if (gear_ratios && cell == '*') {
                        EngineGear& gear = gears[r * width + c];
                        if (gear.count == 0) {
                            gear.first = number;
                        } else if (gear.count == 1) {
                            gear.second = number;
                        }
                        ++gear.count;
                    } else if (!gear_ratios && !IsDigit(cell) && cell != '.') {
                        adjacent = true;
                    }
                }
            }

            if (adjacent) {
                if (total > INT64_MAX - number) {
                    return false;
                }
                total += number;
            }
        }
    }

    for (const EngineGear& gear : gears) {
        if (gear.count == 2) {
            if (gear.second && gear.first > (INT64_MAX - total) / gear.second) {
                return false;
            }
            total += gear.first * gear.second;
        }
    }
    *answer = total;
    return true;
}


bool Day03Part1(const File* input_file, i64* answer)
{
    return SolveDay03(input_file, false, answer);
}


bool Day03Part2(const File* input_file, i64* answer)
{
    return SolveDay03(input_file, true, answer);
}
