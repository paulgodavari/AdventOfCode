// day_02_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


static bool IsSafe(const std::vector<i64>& report, size_t omitted)
{
    i64 previous = 0;
    int direction = 0;
    bool has_previous = false;
    for (size_t index = 0; index < report.size(); ++index) {
        if (index == omitted) {
            continue;
        }
        i64 level = report[index];
        if (has_previous) {
            i64 difference = 0;
            if (__builtin_sub_overflow(level, previous, &difference) || !difference || difference < -3 || difference > 3) {
                return false;
            }
            int current_direction = difference > 0 ? 1 : -1;
            if (direction && direction != current_direction) {
                return false;
            }
            direction = current_direction;
        }

        previous = level;
        has_previous = true;
    }
    return true;
}


static bool SolveDay02(const File* input_file, bool dampener, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<i64> report;
    i64 safe_count = 0;
    while (input.size) {
        String line = ReadLine(&input);
        report.clear();
        i64 level = 0;
        while (ReadInteger(&line, &level)) {
            report.push_back(level);
        }
        if (report.empty()) {
            return false;
        }

        bool safe = IsSafe(report, SIZE_MAX);
        for (size_t index = 0; dampener && !safe && index < report.size(); ++index) {
            safe = IsSafe(report, index);
        }
        safe_count += safe;
    }
    *answer = safe_count;
    return true;
}


bool Day02Part1_2024(const File* input_file, i64* answer)
{
    return SolveDay02(input_file, false, answer);
}


bool Day02Part2_2024(const File* input_file, i64* answer)
{
    return SolveDay02(input_file, true, answer);
}
