// day_11_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <unordered_map>


static bool AddStones(std::unordered_map<u64, i64>* stones, u64 value, i64 count)
{
    i64& total = (*stones)[value];
    return !__builtin_add_overflow(total, count, &total);
}


static bool SolveDay11(const File* input_file, int blinks, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::unordered_map<u64, i64> stones;
    while (input.size) {
        String line = ReadLine(&input);
        while (line.size && (line.start[line.size - 1] == ' ' || line.start[line.size - 1] == '\t')) {
            --line.size;
        }

        while (line.size) {
            i64 value = 0;
            if (!ReadInteger(&line, &value) || value < 0 || !AddStones(&stones, (u64) value, 1)) {
                return false;
            }
        }
    }
    if (stones.empty()) {
        return false;
    }

    std::unordered_map<u64, i64> next;
    for (int blink = 0; blink < blinks; ++blink) {
        next.clear();
        for (const auto& [value, count] : stones) {
            if (!value) {
                if (!AddStones(&next, 1, count)) {
                    return false;
                }
                continue;
            }

            int digits = 1;
            for (u64 remaining = value; remaining >= 10; remaining /= 10) {
                ++digits;
            }
            if (digits % 2 == 0) {
                u64 scale = 1;
                for (int digit = 0; digit < digits / 2; ++digit) {
                    scale *= 10;
                }
                if (!AddStones(&next, value / scale, count) || !AddStones(&next, value % scale, count)) {
                    return false;
                }
            } else {
                u64 product = 0;
                if (__builtin_mul_overflow(value, 2024, &product) || !AddStones(&next, product, count)) {
                    return false;
                }
            }
        }
        stones.swap(next);
    }

    i64 total = 0;
    for (const auto& [value, count] : stones) {
        if (__builtin_add_overflow(total, count, &total)) {
            return false;
        }
    }
    *answer = total;
    return true;
}


bool Day11Part1_2024(const File* input_file, i64* answer)
{
    return SolveDay11(input_file, 25, answer);
}


bool Day11Part2_2024(const File* input_file, i64* answer)
{
    return SolveDay11(input_file, 75, answer);
}
