// day_01_2025.cpp
// AdventOfCode
//
// Copyright (c) 2025 Paul Godavari. All rights reserved.


#include "util/days.h"


static bool SolveDay01(const File* input_file, bool count_clicks, i64* answer)
{
    String input = { input_file->data, input_file->size };
    int position = 50;
    i64 total = 0;
    while (input.size) {
        String line = ReadLine(&input);
        if (!line.size) {
            continue;
        }

        char direction = line.start[0];
        ++line.start;
        --line.size;
        i64 steps = 0;
        if ((direction != 'L' && direction != 'R') || !ReadInteger(&line, &steps) || steps < 0 || line.size) {
            return false;
        }

        int sign = direction == 'L' ? -1 : 1;
        int next = (position + sign * (int) (steps % 100) + 100) % 100;
        i64 count = next == 0;
        if (count_clicks) {
            int first_zero = direction == 'R' ? 100 - position : position;
            if (!first_zero) {
                first_zero = 100;
            }
            count = steps >= first_zero ? 1 + (steps - first_zero) / 100 : 0;
        }
        if (__builtin_add_overflow(total, count, &total)) {
            return false;
        }
        position = next;
    }
    *answer = total;
    return true;
}


bool Day01Part1_2025(const File* input_file, i64* answer)
{
    return SolveDay01(input_file, false, answer);
}


bool Day01Part2_2025(const File* input_file, i64* answer)
{
    return SolveDay01(input_file, true, answer);
}
