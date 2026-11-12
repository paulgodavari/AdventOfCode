// day_09.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


static bool SolveDay09(const File* input_file, bool backwards, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<i64> samples;
    i64 total = 0;
    while (input.size) {
        String line = ReadLine(&input);
        samples.clear();
        i64 number = 0;
        while (ReadInteger(&line, &number)) {
            samples.push_back(number);
        }
        for (size_t index = 0; index < line.size; ++index) {
            if (line.start[index] != ' ' && line.start[index] != '\t') {
                return false;
            }
        }
        if (samples.empty()) {
            return false;
        }

        i64 prediction = 0;
        bool subtract = false;
        for (size_t count = samples.size(); count; --count) {
            i64 value = backwards ? samples[0] : samples[count - 1];
            bool overflow = subtract ? __builtin_sub_overflow(prediction, value, &prediction)
                                     : __builtin_add_overflow(prediction, value, &prediction);
            if (overflow) {
                return false;
            }
            subtract = backwards && !subtract;

            bool all_zero = true;
            for (size_t index = 0; index + 1 < count; ++index) {
                if (__builtin_sub_overflow(samples[index + 1], samples[index], &samples[index])) {
                    return false;
                }
                all_zero = all_zero && samples[index] == 0;
            }
            if (all_zero) {
                break;
            }
        }

        if (__builtin_add_overflow(total, prediction, &total)) {
            return false;
        }
    }

    *answer = total;
    return true;
}


bool Day09Part1(const File* input_file, i64* answer)
{
    return SolveDay09(input_file, false, answer);
}


bool Day09Part2(const File* input_file, i64* answer)
{
    return SolveDay09(input_file, true, answer);
}
