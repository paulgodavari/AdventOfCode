// day_13_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>


static const char* input_file_name = "../../2024/input/day_13.test_input";  // Part 1 = , Part 2 =
// static const char* input_file_name = "../../2024/input/day_13.input";  // Part 1 = , Part 2 =


struct Position
{
    u32 x;
    u32 y;
};


void Day13_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    u64 part1_answer = 0;
    u64 part2_answer = 0;
    
    while (!AtEndOfFile(&parser)) {
        Position a;
        ConsumeString(&parser, CONST_STRING("Button A: X+"));
        a.x = (u32) ParseNumber(&parser);
        Advance(&parser);
        ConsumeString(&parser, CONST_STRING(", Y+"));
        a.y = (u32) ParseNumber(&parser);
        Advance(&parser);
        
        Position b;
        ConsumeString(&parser, CONST_STRING("Button B: X+"));
        b.x = (u32) ParseNumber(&parser);
        Advance(&parser);
        ConsumeString(&parser, CONST_STRING(", Y+"));
        b.y = (u32) ParseNumber(&parser);
        Advance(&parser);
        
        // Prize: X=8400, Y=5400
        Position prize;
        ConsumeString(&parser, CONST_STRING("Prize: X="));
        prize.x = (u32) ParseNumber(&parser);
        Advance(&parser);
        ConsumeString(&parser, CONST_STRING(", Y="));
        prize.y = (u32) ParseNumber(&parser);
        Advance(&parser);
        
        Advance(&parser);
        
        fprintf(stdout, "A = (%u, %u), B = (%u, %u), Prize = (%u, %u)\n", a.x, a.y, b.x, b.y, prize.x, prize.y);
    }

    fprintf(stdout, "2024: Day 13 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2024: Day 13 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
