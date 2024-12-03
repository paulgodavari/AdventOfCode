// day_03_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "util/advent_of_code.h"

#include <vector>


// static const char* input_file_name = "../../2024/input/day_03.test_input";   // Part 1 = 161 
// static const char* input_file_name = "../../2024/input/day_03.test_input2";  // Part 2 = 48
// static const char* input_file_name = "../../2024/input/day_03.input";  // Part 1 = 167650499, Part 2 = 


bool MatchChar(ParseState* parser, char c)
{
    bool match = false;
    
    if (parser->offset < parser->size && parser->data[parser->offset] == c) {
        match = true;
    }
    
    return match;
}


bool MatchDigit(ParseState* parser)
{
    bool match = false;
    
    if (parser->offset < parser->size) {
        if (parser->data[parser->offset] >= '0' && parser->data[parser->offset] <= '9' ) {
            match = true;
        }
    }
    
    return match;
}


void Day03_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    u64 part1_sum = 0;
    u64 part2_sum = 0;
    
    ParseState parser = { input_file.data, input_file.size };

    while (!AtEndOfFile(&parser)) {
        u32 op1 = 0;
        u32 op2 = 0;
        if (ConsumeString(&parser, { "mul", 3 })) {
            if (MatchChar(&parser, '(')) {
                Advance(&parser);
                if (MatchDigit(&parser)) {
                    op1 = ParseNumber(&parser);
                    if (MatchChar(&parser, ',')) {
                        Advance(&parser);
                        if (MatchDigit(&parser)) {
                            op2 = ParseNumber(&parser);
                            if (MatchChar(&parser, ')')) {
                                part1_sum += op1 * op2;
                            }
                        }
                    }
                }
            }
        } else {
            Advance(&parser);
        }
    }

    fprintf(stdout, "2024: Day 03 part 1: %llu\n", part1_sum);
    fprintf(stdout, "2024: Day 03 part 2: %llu\n", part2_sum);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}

