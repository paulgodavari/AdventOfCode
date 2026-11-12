// day_13.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


static size_t FindReflection(const std::vector<String>& rows, bool vertical, int smudges)
{
    size_t length = vertical ? rows[0].size : rows.size();
    size_t across = vertical ? rows.size() : rows[0].size;
    for (size_t split = 1; split < length; ++split) {
        int differences = 0;
        size_t first = split;
        size_t second = split;
        while (first && second < length && differences <= smudges) {
            --first;
            for (size_t index = 0; index < across && differences <= smudges; ++index) {
                char left = vertical ? rows[index].start[first] : rows[first].start[index];
                char right = vertical ? rows[index].start[second] : rows[second].start[index];
                differences += left != right;
            }
            ++second;
        }
        if (differences == smudges) {
            return split;
        }
    }
    return 0;
}


static bool SolveDay13(const File* input_file, int smudges, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<String> rows;
    i64 total = 0;
    while (input.size) {
        rows.clear();
        while (input.size) {
            String line = ReadLine(&input);
            if (!line.size) {
                break;
            }
            if (!rows.empty() && line.size != rows[0].size) {
                return false;
            }
            rows.push_back(line);
        }
        if (rows.empty()) {
            continue;
        }

        i64 score = (i64) FindReflection(rows, true, smudges);
        if (!score) {
            score = 100 * (i64) FindReflection(rows, false, smudges);
        }
        if (!score || __builtin_add_overflow(total, score, &total)) {
            return false;
        }
    }

    *answer = total;
    return true;
}


bool Day13Part1(const File* input_file, i64* answer)
{
    return SolveDay13(input_file, 0, answer);
}


bool Day13Part2(const File* input_file, i64* answer)
{
    return SolveDay13(input_file, 1, answer);
}
