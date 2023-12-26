// day_23.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_23.test_input";  // Part 1: , part 2: 
// static const char* input_file_name = "day_23.input";  // Part 1: , part 2: 


void Day23()
{
    fprintf(stdout, "Day 23\n");
    
    u64 run_time = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    fprintf(stdout, "Initialization time: %.4f\n", MillisecondsSince(run_time));
    
    CloseFile(&input_file);
}
