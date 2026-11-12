// day_01_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <algorithm>
#include <unordered_map>
#include <vector>


static bool ReadLocations(const File* file, std::vector<i64>* left, std::vector<i64>* right)
{
    String input = { file->data, file->size };
    while (input.size) {
        String line = ReadLine(&input);
        i64 first = 0;
        i64 second = 0;
        if (!ReadInteger(&line, &first) || !ReadInteger(&line, &second) || first < 0 || second < 0) {
            return false;
        }
        left->push_back(first);
        right->push_back(second);
    }
    return true;
}


bool Day01Part1_2024(const File* input_file, i64* answer)
{
    std::vector<i64> left;
    std::vector<i64> right;
    if (!ReadLocations(input_file, &left, &right)) {
        return false;
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    i64 total = 0;
    for (size_t index = 0; index < left.size(); ++index) {
        i64 distance = llabs(left[index] - right[index]);
        if (__builtin_add_overflow(total, distance, &total)) {
            return false;
        }
    }
    *answer = total;
    return true;
}


bool Day01Part2_2024(const File* input_file, i64* answer)
{
    std::vector<i64> left;
    std::vector<i64> right;
    if (!ReadLocations(input_file, &left, &right)) {
        return false;
    }

    std::unordered_map<i64, i64> counts;
    for (i64 location : right) {
        ++counts[location];
    }

    i64 total = 0;
    for (i64 location : left) {
        auto count = counts.find(location);
        if (count != counts.end()) {
            i64 score = 0;
            if (__builtin_mul_overflow(location, count->second, &score) || __builtin_add_overflow(total, score, &total)) {
                return false;
            }
        }
    }
    *answer = total;
    return true;
}
