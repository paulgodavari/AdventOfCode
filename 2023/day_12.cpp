// day_12.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <string.h>
#include <vector>


static i64 GenerateMatches(String pattern, size_t offset, const std::vector<i64>& groups, size_t group, i64 broken)
{
    if (offset == pattern.size) {
        if (broken) {
            return group + 1 == groups.size() && broken == groups[group];
        }
        return group == groups.size();
    }

    char spring = pattern.start[offset];
    i64 result = 0;
    if (spring != '#') {
        if (!broken) {
            result = GenerateMatches(pattern, offset + 1, groups, group, 0);
        } else if (group < groups.size() && broken == groups[group]) {
            result = GenerateMatches(pattern, offset + 1, groups, group + 1, 0);
        }
    }
    if (spring != '.' && group < groups.size() && broken < groups[group]) {
        i64 more = GenerateMatches(pattern, offset + 1, groups, group, broken + 1);
        if (result < 0 || more < 0 || __builtin_add_overflow(result, more, &result)) {
            return -1;
        }
    }
    return result;
}


bool Day12Part1(const File* input_file, i64* answer)
{
    String input = { input_file->data, input_file->size };
    i64 total = 0;
    while (input.size) {
        String line = ReadLine(&input);
        size_t size = 0;
        while (size < line.size && (line.start[size] == '.' || line.start[size] == '#' || line.start[size] == '?')) {
            ++size;
        }
        if (!size || size == line.size || line.start[size] != ' ') {
            return false;
        }

        String pattern = { line.start, size };
        line.start += size;
        line.size -= size;
        std::vector<i64> groups;
        for (;;) {
            i64 count = 0;
            if (!ReadInteger(&line, &count) || count <= 0) {
                return false;
            }
            groups.push_back(count);
            if (!line.size) {
                break;
            }
            if (*line.start != ',') {
                return false;
            }
            ++line.start;
            --line.size;
        }

        // Part 2 uses five copies of the pattern separated by '?' and five copies of the groups.
        i64 matches = GenerateMatches(pattern, 0, groups, 0, 0);
        if (matches < 0 || __builtin_add_overflow(total, matches, &total)) {
            return false;
        }
    }
    *answer = total;
    return true;
}
