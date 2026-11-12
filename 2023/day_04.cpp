// day_04.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <string.h>
#include <vector>


static const int kCardNumberCount = 100;


static bool ReadCardNumbers(String text, bool* numbers)
{
    i64 number = 0;
    while (ReadInteger(&text, &number)) {
        if (number < 0 || number >= kCardNumberCount) {
            return false;
        }
        numbers[number] = true;
    }

    for (size_t index = 0; index < text.size; ++index) {
        if (text.start[index] != ' ' && text.start[index] != '\t') {
            return false;
        }
    }
    return true;
}


static bool CountCardMatches(String line, int* matches)
{
    const char* colon = (const char*) memchr(line.start, ':', line.size);
    if (!colon) {
        return false;
    }
    size_t offset = (size_t) (colon + 1 - line.start);
    const char* divider = (const char*) memchr(colon + 1, '|', line.size - offset);
    if (!divider) {
        return false;
    }

    bool winning[kCardNumberCount] = {};
    bool drawn[kCardNumberCount] = {};
    if (!ReadCardNumbers({ colon + 1, (size_t) (divider - colon - 1) }, winning) ||
        !ReadCardNumbers({ divider + 1, (size_t) (line.start + line.size - divider - 1) }, drawn)) {
        return false;
    }

    *matches = 0;
    for (int number = 0; number < kCardNumberCount; ++number) {
        if (winning[number] && drawn[number]) {
            ++*matches;
        }
    }
    return true;
}


bool Day04Part1(const File* input_file, i64* answer)
{
    String input = { input_file->data, input_file->size };
    i64 total = 0;
    while (input.size) {
        int matches = 0;
        if (!CountCardMatches(ReadLine(&input), &matches) || matches > 63) {
            return false;
        }
        i64 points = matches ? (i64) 1 << (matches - 1) : 0;
        if (total > INT64_MAX - points) {
            return false;
        }
        total += points;
    }
    *answer = total;
    return true;
}


bool Day04Part2(const File* input_file, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<int> wins;
    while (input.size) {
        int matches = 0;
        if (!CountCardMatches(ReadLine(&input), &matches)) {
            return false;
        }
        wins.push_back(matches);
    }

    std::vector<i64> copies(wins.size(), 1);
    i64 total = 0;
    for (size_t card = 0; card < wins.size(); ++card) {
        if (total > INT64_MAX - copies[card]) {
            return false;
        }
        total += copies[card];

        for (int next = 1; next <= wins[card] && (size_t) next < wins.size() - card; ++next) {
            if (copies[card + next] > INT64_MAX - copies[card]) {
                return false;
            }
            copies[card + next] += copies[card];
        }
    }

    *answer = total;
    return true;
}
