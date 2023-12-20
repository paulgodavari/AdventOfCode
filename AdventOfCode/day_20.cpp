// day_20.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_20.test_input";  // Part 1: 32000000, part 2:
// static const char* input_file_name = "day_20.test_input2";  // Part 1: 11687500, part 2:
// static const char* input_file_name = "day_20.input";  // Part 1: , part 2:


void Day20()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }

    ParseState parser = { input_file.data, input_file.size, 0 };

    fprintf(stdout, "Day 20 (initialization time: %.4f ms)\n", MillisecondsSince(run_time_start));
    
    CloseFile(&input_file);
}


