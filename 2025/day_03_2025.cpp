// day_03_2025.cpp
// AdventOfCode
//
// Copyright (c) 2025 Paul Godavari. All rights reserved.


#include "util/days.h"


bool Day03Part1_2025(const File* input_file, i64* answer)
{
    String input = { input_file->data, input_file->size };
    i64 total = 0;
    while (input.size) {
        String line = ReadLine(&input);
        if (line.size < 2 || line.start[0] < '0' || line.start[0] > '9') {
            return false;
        }

        int first = line.start[0] - '0';
        int best = 0;
        for (size_t index = 1; index < line.size; ++index) {
            char current = line.start[index];
            if (current < '0' || current > '9') {
                return false;
            }

            int digit = current - '0';
            int pair = first * 10 + digit;
            if (pair > best) {
                best = pair;
            }
            if (digit > first) {
                first = digit;
            }
        }
        if (__builtin_add_overflow(total, best, &total)) {
            return false;
        }
    }
    *answer = total;
    return true;
}
