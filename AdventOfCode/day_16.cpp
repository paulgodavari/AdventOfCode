// day_16.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_16.test_input";  // Part 1: 46, part2: 
// static const char* input_file_name = "day_16.input";  // Part 1: , part2: 


void Day16()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    fprintf(stdout, "Day 16\n");
    
    CloseFile(&input_file);
}


