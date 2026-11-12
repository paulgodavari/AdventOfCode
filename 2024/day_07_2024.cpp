// day_07_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


static bool CanCalibrate(const std::vector<i64>& numbers, size_t index, i64 target, bool concatenate)
{
    if (index == 0) {
        return target == numbers[0];
    }

    i64 last = numbers[index];
    if (target >= last && CanCalibrate(numbers, index - 1, target - last, concatenate)) {
        return true;
    }
    if ((!last && !target) || (last && target % last == 0 && CanCalibrate(numbers, index - 1, target / last, concatenate))) {
        return true;
    }
    if (concatenate) {
        u64 scale = 10;
        for (i64 digits = last; digits >= 10; digits /= 10) {
            scale *= 10;
        }
        if ((u64) target % scale == (u64) last && CanCalibrate(numbers, index - 1, (i64) ((u64) target / scale), concatenate)) {
            return true;
        }
    }
    return false;
}


static bool SolveDay07(const File* input_file, bool concatenate, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<i64> numbers;
    i64 total = 0;
    while (input.size) {
        String line = ReadLine(&input);
        i64 target = 0;
        if (!ReadInteger(&line, &target) || target < 0 || !ConsumeString(&line, CONST_STRING(":"))) {
            return false;
        }

        numbers.clear();
        i64 number = 0;
        while (ReadInteger(&line, &number)) {
            if (number < 0) {
                return false;
            }
            numbers.push_back(number);
        }
        if (numbers.empty()) {
            return false;
        }

        if (CanCalibrate(numbers, numbers.size() - 1, target, concatenate) && __builtin_add_overflow(total, target, &total)) {
            return false;
        }
    }
    *answer = total;
    return true;
}


bool Day07Part1_2024(const File* input_file, i64* answer)
{
    return SolveDay07(input_file, false, answer);
}


bool Day07Part2_2024(const File* input_file, i64* answer)
{
    return SolveDay07(input_file, true, answer);
}
