// day_13_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>


// static const char* input_file_name = "../../2024/input/day_13.test_input";  // Part 1 = 480, Part 2 = 875318608908
static const char* input_file_name = "../../2024/input/day_13.input";  // Part 1 = 29517, Part 2 = 103570327981381


struct Position
{
    u64 x;
    u64 y;
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
    u64 offset = 10000000000000;
    
    while (!AtEndOfFile(&parser)) {
        Position a;
        ConsumeString(&parser, CONST_STRING("Button A: X+"));
        a.x = ParseNumber(&parser);
        Advance(&parser);
        ConsumeString(&parser, CONST_STRING(", Y+"));
        a.y = ParseNumber(&parser);
        Advance(&parser);
        
        Position b;
        ConsumeString(&parser, CONST_STRING("Button B: X+"));
        b.x = ParseNumber(&parser);
        Advance(&parser);
        ConsumeString(&parser, CONST_STRING(", Y+"));
        b.y = ParseNumber(&parser);
        Advance(&parser);
        
        // Prize: X=8400, Y=5400
        Position prize;
        ConsumeString(&parser, CONST_STRING("Prize: X="));
        prize.x = ParseNumber(&parser);
        Advance(&parser);
        ConsumeString(&parser, CONST_STRING(", Y="));
        prize.y = ParseNumber(&parser);
        Advance(&parser);
        
        Advance(&parser);
        
        Position prize2 = { prize.x + offset, prize.y + offset };
        
        // fprintf(stdout, "A = (%u, %u), B = (%u, %u), Prize = (%u, %u)\n", a.x, a.y, b.x, b.y, prize.x, prize.y);
        
        // Each problem is 2 equations with 2 unknowns:
        //    A*a.x + B*b.x = prize.x
        //    A*a.y + B*b.y = prize.y
        //
        // Solve using Cramer's rule
        //    A = dx / d
        //    B = dy / d
        // where d is the determinant: a.x*b.y - a.y*bx

        i64 d = a.x * b.y - a.y * b.x;
        i64 dx = prize.x * b.y - prize.y * b.x;
        i64 dy = a.x * prize.y - a.y * prize.x;

        i64 dx2 = prize2.x * b.y - prize2.y * b.x;
        i64 dy2 = a.x * prize2.y - a.y * prize2.x;

        if (d) {
            // Part 1
            i64 A = dx / d;
            i64 B = dy / d;
            if (((A * a.x + B * b.x) == prize.x) && ((A * a.y + B * b.y) == prize.y)) {
                // Make sure that A and B are integers.
                part1_answer += 3 * A + B;
            }
            
            // Part 2
            i64 A2 = dx2 / d;
            i64 B2 = dy2 / d;
            if (((A2 * a.x + B2 * b.x) == prize2.x) && ((A2 * a.y + B2 * b.y) == prize2.y)) {
                // Make sure that A and B are integers.
                part2_answer += 3 * A2 + B2;
            }
        }
    }

    fprintf(stdout, "2024: Day 13 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2024: Day 13 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
