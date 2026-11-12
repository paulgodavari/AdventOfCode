// day_01.cpp
// AdventOfCode
//
// Solution for: https://adventofcode.com/2023/day/1
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <string.h>


static const String kNumbers[] = {
    CONST_STRING("zero"),
    CONST_STRING("one"),
    CONST_STRING("two"),
    CONST_STRING("three"),
    CONST_STRING("four"),
    CONST_STRING("five"),
    CONST_STRING("six"),
    CONST_STRING("seven"),
    CONST_STRING("eight"),
    CONST_STRING("nine")
};


static int GetCalibrationValueFromLine(const char* line, size_t line_size, bool use_words)
{
    int first_digit = -1;
    int last_digit = -1;

    // Advance one character at a time so number words can overlap.
    for (size_t index = 0; index < line_size; ++index) {
        int digit = -1;
        char character = line[index];
        if (character >= '0' && character <= '9') {
            digit = character - '0';
        } else if (use_words) {
            for (int number = 0; number < 10; ++number) {
                String keyword = kNumbers[number];
                if (keyword.size <= line_size - index && MatchNumberKeyword(line + index, 0, keyword)) {
                    digit = number;
                    break;
                }
            }
        }

        if (digit >= 0) {
            if (first_digit < 0) {
                first_digit = digit;
            }
            last_digit = digit;
        }
    }

    return first_digit < 0 ? -1 : first_digit * 10 + last_digit;
}


static bool SolveDay01(const File* input_file, bool use_words, i64* answer)
{
    i64 total = 0;
    size_t offset = 0;
    while (offset < input_file->size) {
        const char* line = input_file->data + offset;
        size_t remaining = input_file->size - offset;
        const char* line_end = (const char*) memchr(line, '\n', remaining);
        size_t line_size = line_end ? (size_t) (line_end - line) : remaining;

        int calibration_value = GetCalibrationValueFromLine(line, line_size, use_words);
        if (calibration_value < 0 || total > INT64_MAX - calibration_value) {
            return false;
        }

        total += calibration_value;
        offset += line_size + (line_end ? 1 : 0);
    }

    *answer = total;
    return true;
}


bool Day01Part1(const File* input_file, i64* answer)
{
    return SolveDay01(input_file, false, answer);
}


bool Day01Part2(const File* input_file, i64* answer)
{
    return SolveDay01(input_file, true, answer);
}
