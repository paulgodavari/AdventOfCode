// day_13.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_13.test_input";
// static const char* input_file_name = "day_13.input";


void Day13()
{
    fprintf(stdout, "Day 13\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    u32 part_1_sum = 0;
    
    
    fprintf(stdout, "Part 1: %u\n", part_1_sum);
    
    CloseFile(&input_file);
}


