// day_01_2025.cpp
// AdventOfCode
//
// Copyright (c) 2025 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


// static const char* input_file_name = "../../2025/input/day_01.test_input";  // Part 1 = 3, Part 2 = 6
static const char* input_file_name = "../../2025/input/day_01.input";  // Part 1 = 1152, Part 2 = 6671


void Day01_2025()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    u64 part1_answer = 0;
    u64 part2_answer = 0;
    
    int position = 50;
    char direction = 0;
    int steps = 0;
    
    rewind(input_file.handle);
    
    while (fscanf(input_file.handle, " %c%d", &direction, &steps) == 2) {
        if (direction == 'L' || direction == 'R') {
            int sign = direction == 'L' ? -1 : 1;
            
            // Calculate the new position.
            int new_position = (position + sign * steps) % 100;
            if (new_position < 0) {
                new_position += 100;
            }
            
            // Part 1 & 2: count the number of times we end up at zero.
            if (new_position == 0) {
                part1_answer++;
                part2_answer++;
            }
            
            // Part 2: we pass zero for each full rotation.
            int full_rotations = steps / 100;
            part2_answer += full_rotations;
            
            // Part 2: if our new position is outside the range 0..99, we've passed zero,
            //         excluding the case where we start at zero and go left.
            int unadjusted_position = position + (steps % 100) * sign;
            if ((unadjusted_position < 0 && position != 0) || (unadjusted_position > 100)) {
                part2_answer++;
            }

            position = new_position;
        } else {
            fprintf(stderr, "Bad direction: %d\n", direction);
            break;
        }
    }

    fprintf(stdout, "2025: Day 13 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2025: Day 13 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
