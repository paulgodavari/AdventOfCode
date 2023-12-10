// day_10.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_10.test_input";
// static const char* input_file_name = "day_10.input";


void Day10()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stdout, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    CloseFile(&input_file);
}
