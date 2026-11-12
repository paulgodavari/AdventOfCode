// day_13_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"


struct ClawPosition
{
    i64 x;
    i64 y;
};


static bool ReadPosition(String line, String x_prefix, String y_prefix, ClawPosition* position)
{
    return ConsumeString(&line, x_prefix) && ReadInteger(&line, &position->x) &&
           ConsumeString(&line, y_prefix) && ReadInteger(&line, &position->y) && !line.size;
}


static bool SolveDay13(const File* input_file, bool move_prize, i64* answer)
{
    const i64 kPrizeOffset = 10000000000000;
    String input = { input_file->data, input_file->size };
    i64 total = 0;
    while (input.size) {
        String line = ReadLine(&input);
        if (!line.size) {
            continue;
        }

        ClawPosition a = {};
        ClawPosition b = {};
        ClawPosition prize = {};
        if (!ReadPosition(line, CONST_STRING("Button A: X"), CONST_STRING(", Y"), &a) ||
            !ReadPosition(ReadLine(&input), CONST_STRING("Button B: X"), CONST_STRING(", Y"), &b) ||
            !ReadPosition(ReadLine(&input), CONST_STRING("Prize: X="), CONST_STRING(", Y="), &prize)) {
            return false;
        }
        if (move_prize && (__builtin_add_overflow(prize.x, kPrizeOffset, &prize.x) ||
                           __builtin_add_overflow(prize.y, kPrizeOffset, &prize.y))) {
            return false;
        }

        // Solve for the two button counts with Cramer's rule.
        __int128 determinant = (__int128) a.x * b.y - (__int128) a.y * b.x;
        __int128 a_numerator = (__int128) prize.x * b.y - (__int128) prize.y * b.x;
        __int128 b_numerator = (__int128) a.x * prize.y - (__int128) a.y * prize.x;
        if (!determinant || a_numerator % determinant || b_numerator % determinant) {
            continue;
        }

        __int128 a_presses = a_numerator / determinant;
        __int128 b_presses = b_numerator / determinant;
        if (a_presses < 0 || b_presses < 0 || (!move_prize && (a_presses > 100 || b_presses > 100))) {
            continue;
        }
        if (a_presses > INT64_MAX / 3 || b_presses > INT64_MAX) {
            return false;
        }

        i64 cost = 0;
        if (__builtin_add_overflow(3 * (i64) a_presses, (i64) b_presses, &cost) || __builtin_add_overflow(total, cost, &total)) {
            return false;
        }
    }
    *answer = total;
    return true;
}


bool Day13Part1_2024(const File* input_file, i64* answer)
{
    return SolveDay13(input_file, false, answer);
}


bool Day13Part2_2024(const File* input_file, i64* answer)
{
    return SolveDay13(input_file, true, answer);
}
