// day_22.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_22.test_input";  // Part 1: 5, part 2:
// static const char* input_file_name = "day_22.input";  // Part 1: , part 2:


void Day22()
{
    fprintf(stdout, "Day 22\n");

    u64 run_time = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    fprintf(stdout, "Initialization time: %.4f\n", MillisecondsSince(run_time));
    
    // Part 1
    u64 start_time = TimeNow();
    u32 part_1_bricks = 0;
    fprintf(stdout, "Part 1 number of bricks: %u (%.4f ms)\n", part_1_bricks, MillisecondsSince(start_time));
    fprintf(stdout, "Run time: %.4f ms\n", MillisecondsSince(run_time));
    
    CloseFile(&input_file);
}


