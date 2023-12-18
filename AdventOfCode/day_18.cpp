// day_18.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_18.test_input";  // Part 1: , part 2:
// static const char* input_file_name = "day_18.input";  // Part 1: , part 2:


void Day18()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    fprintf(stdout, "Day 18\n");
    
    
    CloseFile(&input_file);
}


