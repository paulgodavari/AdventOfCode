// day_01_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <vector>
#include <unordered_map>


//static const char* input_file_name = "../../2024/input/day_01.test_input";  // Part 1:   Value =  11, Part 2:  Value = 31
static const char* input_file_name = "../../2024/input/day_01.input";  // Part 1: Value = 1830467, Part 2:  Value = 26674158


void Day01_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    std::vector<i32> v1;
    std::vector<i32> v2;
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    while (!AtEndOfFile(&parser) && !AtEndOfLine(&parser)) {
        i32 i1 = ParseNumber(&parser);
        v1.push_back(i1);
        i32 i2 = ParseNumber(&parser);
        v2.push_back(i2);
        Advance(&parser);
        //fprintf(stdout, "%u: %u\n", i1, i2);
    }
    
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    
    u64 part1_distance = 0;
    for (int index = 0; index < v1.size(); ++index) {
        part1_distance += abs(v1[index] - v2[index]);
    }
    
    std::unordered_map<u32, u32> h2;
    for (int index = 0; index < v2.size(); ++index) {
        i32 value = v2[index];
        auto it = h2.find(value);
        if (it != h2.end()) {
            h2[value] = it->second + 1;
        } else {
            h2[value] = 1;
        }
    }
    
    u64 part2_distance = 0;
    for (int index = 0; index < v1.size(); ++index) {
        u32 current = v1[index];
        auto it = h2.find(current);
        if (it != h2.end()) {
            part2_distance += current * it->second;
        }
    }
    
    fprintf(stdout, "2024: Day 01 part 1: %llu\n", part1_distance);
    fprintf(stdout, "2024: Day 01 part 2: %llu\n", part2_distance);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}

