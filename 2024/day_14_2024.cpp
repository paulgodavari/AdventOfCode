// day_14_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"


static int PositionAfter100Seconds(i64 position, i64 velocity, int size)
{
    i64 end = (position % size + 100 * (velocity % size)) % size;
    return (int) (end < 0 ? end + size : end);
}


static bool SolveDay14Part1(const File* input_file, int width, int height, i64* answer)
{
    String input = { input_file->data, input_file->size };
    i64 quadrants[4] = {};
    while (input.size) {
        String line = ReadLine(&input);
        i64 x = 0;
        i64 y = 0;
        i64 dx = 0;
        i64 dy = 0;
        if (!ConsumeString(&line, CONST_STRING("p=")) ||
            !ReadInteger(&line, &x) || !ConsumeString(&line, CONST_STRING(",")) || !ReadInteger(&line, &y) ||
            !ConsumeString(&line, CONST_STRING(" v=")) ||
            !ReadInteger(&line, &dx) || !ConsumeString(&line, CONST_STRING(",")) || !ReadInteger(&line, &dy) || line.size) {
            return false;
        }

        int col = PositionAfter100Seconds(x, dx, width);
        int row = PositionAfter100Seconds(y, dy, height);
        if (col != width / 2 && row != height / 2) {
            int quadrant = (row > height / 2) * 2 + (col > width / 2);
            ++quadrants[quadrant];
        }
    }

    i64 safety_factor = 1;
    for (i64 count : quadrants) {
        if (__builtin_mul_overflow(safety_factor, count, &safety_factor)) {
            return false;
        }
    }
    *answer = safety_factor;
    return true;
}


bool Day14Part1_2024(const File* input_file, i64* answer)
{
    return SolveDay14Part1(input_file, 101, 103, answer);
}


bool Day14Part1Test_2024(const File* input_file, i64* answer)
{
    return SolveDay14Part1(input_file, 11, 7, answer);
}
