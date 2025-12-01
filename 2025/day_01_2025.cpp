// day_01_2025.cpp
// AdventOfCode
//
// Copyright (c) 2025 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

static const char* input_file_name = "../../2025/input/day_01.test_input";  // Part 1 = 3, Part 2 =
// static const char* input_file_name = "../../2025/input/day_01.input";  // Part 1 = , Part 2 =


void Day01_2025()
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
    
    int position = 50;
    char direction = 0;
    int steps = 0;
    
    rewind(input_file.handle);
    
    while (fscanf(input_file.handle, " %c%d", &direction, &steps) == 2) {
        if (direction == 'L' || direction == 'R') {
            int sign = direction == 'L' ? -1 : 1;
            position += sign * steps;
            if (position == 0) {
                part1_answer++;
            }
        } else {
            fprintf(stderr, "Bad direction: %d\n", direction);
            break;
        }
    }

    fprintf(stdout, "2025: Day 13 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2025: Day 13 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
