// day_09.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_09.test_input";  // Part 1: , part 2:
// static const char* input_file_name = "day_09.input";  // Part 1: , part 2:


static const int kMaxSamples = 100;
static const int kMaxRows = 200;


struct History
{
    u32 count;
    i32 samples[kMaxSamples];
};


void Day09()
{
    fprintf(stdout, "Day 09\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    History table[kMaxRows] = {};
    
    u32 line_count = 0;
    while (!AtEndOfFile(&parser)) {
        History* history = &table[line_count];
        while (!AtEndOfLine(&parser)) {
            history->samples[history->count] = ParseNumber(&parser);
            history->count++;
        }
        Advance(&parser);
        line_count++;
    }
    
    for (int row = 0; row < line_count; ++row) {
        fprintf(stdout, "%u: ", row);
        History* history = &table[row];
        for (int i = 0; i < history->count; ++i) {
            fprintf(stdout, "%u, ", history->samples[i]);
        }
        fprintf(stdout, "\n");
    }
}


