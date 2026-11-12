// day_10.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.
//
// Raycast method: https://www.reddit.com/r/adventofcode/comments/18ey1s7/comment/kcr1jga/


#include "util/days.h"

#include <vector>


struct PipePosition
{
    int row;
    int col;
};


static const int kUp = 0;
static const int kRight = 1;
static const int kDown = 2;
static const int kLeft = 3;
static const PipePosition kDirections[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };


static int PipeConnections(char pipe)
{
    switch (pipe) {
        case '|': return (1 << kUp) | (1 << kDown);
        case '-': return (1 << kLeft) | (1 << kRight);
        case 'L': return (1 << kUp) | (1 << kRight);
        case 'J': return (1 << kUp) | (1 << kLeft);
        case '7': return (1 << kDown) | (1 << kLeft);
        case 'F': return (1 << kDown) | (1 << kRight);
    }
    return 0;
}


static PipePosition Step(PipePosition position, int direction)
{
    PipePosition delta = kDirections[direction];
    return { position.row + delta.row, position.col + delta.col };
}


static bool InBounds(PipePosition position, int height, int width)
{
    return position.row >= 0 && position.row < height && position.col >= 0 && position.col < width;
}


static bool SolveDay10(const File* input_file, bool count_inside, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<String> rows;
    PipePosition start = { -1, -1 };
    while (input.size) {
        String row = ReadLine(&input);
        if (!row.size || row.size > INT32_MAX || rows.size() >= INT32_MAX || (!rows.empty() && row.size != rows[0].size)) {
            return false;
        }
        for (size_t col = 0; col < row.size; ++col) {
            if (row.start[col] == 'S') {
                if (start.row >= 0) {
                    return false;
                }
                start = { (int) rows.size(), (int) col };
            }
        }
        rows.push_back(row);
    }
    if (start.row < 0) {
        return false;
    }

    int height = (int) rows.size();
    int width = (int) rows[0].size;
    int start_mask = 0;
    int connections = 0;
    int direction = 0;
    for (int next = 0; next < 4; ++next) {
        PipePosition neighbor = Step(start, next);
        int back = 1 << ((next + 2) % 4);
        if (InBounds(neighbor, height, width) && (PipeConnections(rows[neighbor.row].start[neighbor.col]) & back)) {
            start_mask |= 1 << next;
            direction = next;
            ++connections;
        }
    }
    if (connections != 2) {
        return false;
    }

    std::vector<u8> loop((size_t) height * width);
    loop[(size_t) start.row * width + start.col] = start_mask;
    PipePosition position = start;
    i64 steps = 0;
    for (;;) {
        position = Step(position, direction);
        ++steps;
        if (!InBounds(position, height, width)) {
            return false;
        }
        int back = 1 << ((direction + 2) % 4);
        if (position.row == start.row && position.col == start.col) {
            if (!(start_mask & back)) {
                return false;
            }
            break;
        }

        u8& visited = loop[(size_t) position.row * width + position.col];
        int mask = PipeConnections(rows[position.row].start[position.col]);
        if (visited || !(mask & back)) {
            return false;
        }
        visited = mask;
        direction = __builtin_ctz((unsigned int) (mask & ~back));
    }

    if (!count_inside) {
        *answer = steps / 2;
        return true;
    }

    i64 inside_count = 0;
    const int vertical = (1 << kUp) | (1 << kDown);
    for (int row = 0; row < height; ++row) {
        bool inside = false;
        int opening_bend = 0;
        for (int col = 0; col < width; ++col) {
            int mask = loop[(size_t) row * width + col];
            int bend = mask & vertical;
            if (!mask) {
                inside_count += inside;
            } else if (mask == vertical) {
                inside = !inside;
            } else if (bend && (mask & (1 << kRight))) {
                opening_bend = bend;
            } else if (bend && (mask & (1 << kLeft))) {
                // Bends pointing in opposite directions cross the scan line.
                if (opening_bend != bend) {
                    inside = !inside;
                }
                opening_bend = 0;
            }
        }
    }

    *answer = inside_count;
    return true;
}


bool Day10Part1(const File* input_file, i64* answer)
{
    return SolveDay10(input_file, false, answer);
}


bool Day10Part2(const File* input_file, i64* answer)
{
    return SolveDay10(input_file, true, answer);
}
