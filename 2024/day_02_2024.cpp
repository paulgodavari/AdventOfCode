// day_02_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "util/advent_of_code.h"

#include <vector>


// static const char* input_file_name = "../../2024/input/day_02.test_input";  // Part 1 =  2, Part 2 = 4
static const char* input_file_name = "../../2024/input/day_02.input";  // Part 1 = 220, Part 2 = 296


bool IsSafe(const std::vector<u32>& reports)
{
    bool result = true;
    
    bool initial_increasing = reports[0] < reports[1];
    for (int index = 0; index < reports.size() - 1; ++index) {
        bool new_increasing = reports[index] < reports[index + 1];
        u32 diff = abs((i32)reports[index] - (i32)reports[index + 1]);
        if (initial_increasing != new_increasing || diff > 3 || reports[index] == reports[index + 1]) {
            result = false;
        }
    }
    
    return result;
}


void Day02_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    u32 part1_sum = 0;
    u32 part2_sum = 0;
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    while (!AtEndOfFile(&parser)) {
        
        std::vector<u32> reports;
        while (!AtEndOfLine(&parser) && !AtEndOfFile(&parser)) {
            u32 report = (u32) ParseNumber(&parser);
            reports.push_back(report);
        }
        Advance(&parser);
        
        bool safe = IsSafe(reports);
        if (safe) {
            part1_sum++;
            part2_sum++;
        }
        
        // Part 2: Remove a single entry in an unsafe report to see.
        if (!safe) {
            for (int index = 0; index < reports.size(); ++index) {
                std::vector<u32> one_removed = reports;
                one_removed.erase(one_removed.begin() + index);
                bool new_safe = IsSafe(one_removed);
                if (new_safe) {
                    part2_sum++;
                    break;
                }
            }
        }
    }

    fprintf(stdout, "2024: Day 02 part 1: %u\n", part1_sum);
    fprintf(stdout, "2024: Day 02 part 2: %u\n", part2_sum);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}

