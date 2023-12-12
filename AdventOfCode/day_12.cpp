// day_12.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_12.test_input";  // Part 1: 21, part 2:
// static const char* input_file_name = "day_12.input";  // Part 1: , part 2:

static const u32 kMaxGroups = 10;


void Day12()
{
    fprintf(stdout, "Day 12\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading file: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    i32 pattern_count = 0;
    u32 combination_sum = 0;
    
    while (!AtEndOfFile(&parser)) {
        String pattern = ParseUntilSpace(&parser);
        fprintf(stdout, "%d: %.*s  ->  ", pattern_count, (int) pattern.size, pattern.start);
        u32 groups[kMaxGroups] = {};
        u32 group_count = 0;
        while (!AtEndOfLine(&parser)) {
            groups[group_count] = ParseNumber(&parser);
            fprintf(stdout, "%u  ", groups[group_count]);
            group_count++;
        }
        fprintf(stdout, "(%u)\n", group_count);
        Advance(&parser);
        pattern_count++;
    }
    
    fprintf(stdout, "Day 12 combination sum: %u\n", combination_sum);

    CloseFile(&input_file);
}
