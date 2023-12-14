// day_14.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_14.test_input";  // Part 1: , part 2:
// static const char* input_file_name = "day_14.input";  // Part 1: , part 2:


void Day14()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    fprintf(stdout, "Day 14\n");
    
    CloseFile(&input_file);
}


