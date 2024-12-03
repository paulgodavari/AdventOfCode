// day_01_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <vector>


// static const char* input_file_name = "../../2024/input/day_01.test_input";  // Part 1:   Value =  11
static const char* input_file_name = "../../2024/input/day_01.input";  // Part 1: Value = 1830467


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
        fprintf(stdout, "%u: %u\n", i1, i2);
    }
    
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    
    u64 distance = 0;
    for (int index = 0; index < v1.size(); ++index) {
        distance += abs(v1[index] - v2[index]);
    }
    
    fprintf(stdout, "2024: Day 01 part 1: %llu\n", distance);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}

