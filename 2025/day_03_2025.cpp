// day_03_2025.cpp
// AdventOfCode
//
// Copyright (c) 2025 Paul Godavari. All rights reserved.


#include "advent_of_code.h"


// static const char* input_file_name = "../../2025/input/day_03.test_input";  // Part 1 = 357, Part 2 =
static const char* input_file_name = "../../2025/input/day_03.input";  // Part 1 = 17443, Part 2 =


void Day03_2025()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    u64 part1_answer = 0;
    u64 part2_answer = 0;
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    i32 line_length = 0;
    while (!AtEndOfLine(&parser)) {
        line_length++;
        Advance(&parser);
    }
    parser.offset = 0;
    fprintf(stdout, "Line length = %d\n", line_length);

    while (!AtEndOfFile(&parser)) {
        i32 start_max = parser.data[parser.offset] - (i32) '0';
        i32 end_max = -1;
        Advance(&parser);
        
        for (i32 index = 0; index < line_length - 2; index++) {
            i32 current = parser.data[parser.offset] - (i32) '0';
            if (current > start_max) {
                start_max = current;
                end_max = -1;  // We've moved the start, need to force the end to update.
            } else if (current > end_max) {
                end_max = current;
            }
            Advance(&parser);
        }

        i32 current = parser.data[parser.offset] - (i32) '0';
        if (end_max == -1 || current > end_max) {
            end_max = current;
        }

        Advance(&parser);
        Advance(&parser);
        
        part1_answer += start_max * 10 + end_max;
    }
    
    fprintf(stdout, "2025: Day 03 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2025: Day 03 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
