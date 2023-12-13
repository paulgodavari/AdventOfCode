// day_13.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_13.test_input";
// static const char* input_file_name = "day_13.input";


struct Pattern
{
    char* start;
    u32 rows;
    u32 cols;
};


struct Reflection
{
    u32 row;
    u32 col;
};


Reflection FindReflection(ParseState* parser, Pattern pattern)
{
    Reflection result = {};
    
    // Find vertical reflection.
    for (int col = 0; col < (pattern.cols - 1); ++col) {
        char* c1_start = pattern.start + col;  // 1st column to compare
        char* c2_start = c1_start + 1;         // 2nd column to compare
        bool found_reflection = true;
        for (int row = 0; row < pattern.rows; ++row) {
            char c1 = *(c1_start + row * (pattern.cols + 1));  // +1 to acount for the trailing '\n' on each line
            char c2 = *(c2_start + row * (pattern.cols + 1));  // +1 to acount for the trailing '\n' on each line
            if (c1 != c2) {
                // Columns are different.
                found_reflection = false;
                break;
            }
        }
        if (found_reflection) {
            result.col = col;
            break;
        }
    }
    
    return result;
}


void Day13()
{
    fprintf(stdout, "Day 13\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    u32 part_1_sum = 0;
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    while (!AtEndOfFile(&parser)) {
        
        // Read each pattern.
        Pattern pattern = { parser.data + parser.offset, 0, 0 };
        bool reading_pattern = true;
        u32 col_count = 0;
        while (!AtEndOfLine(&parser) && reading_pattern) {
            col_count++;
            Advance(&parser);
            if (AtEndOfLine(&parser)) {
                if (pattern.cols == 0) {
                    pattern.cols = col_count;
                }
                col_count = 0;
                pattern.rows++;
                Advance(&parser);
                if (AtEndOfLine(&parser)) {
                    // Found a blank line indicating end of pattern.
                    Advance(&parser);
                    reading_pattern = false;
                }
                if (AtEndOfFile(&parser)) {
                    // Last pattern found.
                    reading_pattern = false;
                }
            }
        }
        
        Reflection reflection_point = FindReflection(&parser, pattern);
        part_1_sum += reflection_point.col;
        
        fprintf(stdout, "Found pattern sum: %u (r: %u, c: %u)\n", reflection_point.col, pattern.rows, pattern.cols);
    }
    
    fprintf(stdout, "Part 1: %u\n", part_1_sum);
    
    CloseFile(&input_file);
}


