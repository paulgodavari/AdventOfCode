// day_03_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"


static bool ReadOperand(String* input, i64* value)
{
    size_t digits = 0;
    i64 number = 0;
    while (digits < 3 && digits < input->size && input->start[digits] >= '0' && input->start[digits] <= '9') {
        number = number * 10 + input->start[digits++] - '0';
    }
    if (!digits) {
        return false;
    }

    input->start += digits;
    input->size -= digits;
    *value = number;
    return true;
}


static bool MatchMul(String* input, i64* product)
{
    String instruction = *input;
    i64 left = 0;
    i64 right = 0;
    if (!ConsumeString(&instruction, CONST_STRING("mul(")) ||
        !ReadOperand(&instruction, &left) || !ConsumeString(&instruction, CONST_STRING(",")) ||
        !ReadOperand(&instruction, &right) || !ConsumeString(&instruction, CONST_STRING(")"))) {
        return false;
    }

    *input = instruction;
    *product = left * right;
    return true;
}


static bool SolveDay03(const File* input_file, bool use_switches, i64* answer)
{
    String input = { input_file->data, input_file->size };
    bool enabled = true;
    i64 total = 0;
    while (input.size) {
        i64 product = 0;
        if (use_switches && ConsumeString(&input, CONST_STRING("don't()"))) {
            enabled = false;
        } else if (use_switches && ConsumeString(&input, CONST_STRING("do()"))) {
            enabled = true;
        } else if (MatchMul(&input, &product)) {
            if (enabled && __builtin_add_overflow(total, product, &total)) {
                return false;
            }
        } else {
            ++input.start;
            --input.size;
        }
    }
    *answer = total;
    return true;
}


bool Day03Part1_2024(const File* input_file, i64* answer)
{
    return SolveDay03(input_file, false, answer);
}


bool Day03Part2_2024(const File* input_file, i64* answer)
{
    return SolveDay03(input_file, true, answer);
}
