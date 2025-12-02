// day_02_2025.cpp
// AdventOfCode
//
// Copyright (c) 2025 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "../../2025/input/day_02.test_input";  // Part 1 = 1227775554, Part 2 =
// static const char* input_file_name = "../../2025/input/day_02.input";  // Part 1 = , Part 2 =


void Day02_2025()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    u64 part1_answer = 0;
    u64 part2_answer = 0;
    
    ParseState parser = { input_file.data, input_file.size, 0 };

    
    fprintf(stdout, "2025: Day 02 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2025: Day 02 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
