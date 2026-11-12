// day_18.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


struct DigInstruction
{
    int direction;
    i64 distance;
};


static const i64 kRowStep[] = { -1, 0, 1, 0 };
static const i64 kColStep[] = { 0, 1, 0, -1 };


bool Day18Part1(const File* input_file, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<DigInstruction> instructions;
    i64 row = 0;
    i64 col = 0;
    i64 top = 0;
    i64 bottom = 0;
    i64 left = 0;
    i64 right = 0;
    while (input.size) {
        String line = ReadLine(&input);
        if (!line.size) {
            return false;
        }

        int direction = 0;
        switch (*line.start) {
            case 'U': direction = 0; break;
            case 'R': direction = 1; break;
            case 'D': direction = 2; break;
            case 'L': direction = 3; break;
            default: return false;
        }
        ++line.start;
        --line.size;
        i64 distance = 0;
        if (!ReadInteger(&line, &distance) || distance <= 0 ||
            __builtin_add_overflow(row, kRowStep[direction] * distance, &row) ||
            __builtin_add_overflow(col, kColStep[direction] * distance, &col)) {
            return false;
        }

        instructions.push_back({ direction, distance });
        if (row < top) top = row;
        if (row > bottom) bottom = row;
        if (col < left) left = col;
        if (col > right) right = col;
    }
    if (instructions.empty() || row || col) {
        return false;
    }

    // Part 2's first five color digits encode the distance. The last digit gives the direction:
    // 0 right, 1 down, 2 left, 3 up.
    i64 height = 0;
    i64 width = 0;
    i64 area = 0;
    if (__builtin_sub_overflow(bottom, top, &height) || __builtin_add_overflow(height, 1, &height) ||
        __builtin_sub_overflow(right, left, &width) || __builtin_add_overflow(width, 1, &width) ||
        __builtin_mul_overflow(height, width, &area)) {
        return false;
    }

    std::vector<u8> trench((size_t) area);
    row = -top;
    col = -left;
    for (DigInstruction instruction : instructions) {
        int forward = 1 << instruction.direction;
        int back = 1 << ((instruction.direction + 2) % 4);
        for (i64 step = 0; step < instruction.distance; ++step) {
            trench[(size_t) (row * width + col)] |= forward;
            row += kRowStep[instruction.direction];
            col += kColStep[instruction.direction];
            trench[(size_t) (row * width + col)] |= back;
        }
    }

    i64 total = 0;
    for (i64 y = 0; y < height; ++y) {
        bool inside = false;
        for (i64 x = 0; x < width; ++x) {
            u8 connections = trench[(size_t) (y * width + x)];
            if (connections & 1) {
                inside = !inside;
            }
            total += connections || inside;
        }
    }

    *answer = total;
    return true;
}
