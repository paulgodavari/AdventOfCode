// day_11_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <vector>


// static const char* input_file_name = "../../2024/input/day_11.test_input";  // Part 1 = 55312, Part 2 =
static const char* input_file_name = "../../2024/input/day_11.input";  // Part 1 = 198075, Part 2 =


struct Digits
{
    u64 left;
    u64 right;
};


u32 NumberOfDigits(u64 number)
{
    u32 result = 0;
    
    while (number > 0) {
        result += 1;
        number /= 10;
    }
    
    return result;
}


Digits SplitNumberInHalf(u64 number)
{
    Digits result = {};
    
    u32 digits = NumberOfDigits(number) / 2;
    
    for (int i = 0; i < digits; ++i) {
        result.right += (number % 10) * std::pow(10, i);
        number /= 10;
    }
    for (int i = 0; i < digits; ++i) {
        result.left += (number % 10) * std::pow(10, i);
        number /= 10;
    }

    return result;
}


void Day11_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    std::vector<u64> stones;
    while (!AtEndOfLine(&parser)) {
        u64 number = ParseNumber(&parser);
        stones.push_back(number);
    }
    
    u32 blinks = 0;
    while (blinks < 25) {
        std::vector<u64> new_stones;
        for (int index = 0; index < stones.size(); ++index) {
            u64 stone = stones[index];
            if (stone == 0) {
                new_stones.push_back(1);
            } else if (NumberOfDigits(stone) % 2 == 0) {
                Digits digits = SplitNumberInHalf(stone);
                new_stones.push_back(digits.left);
                new_stones.push_back(digits.right);
            } else {
                new_stones.push_back(stone * 2024);
            }
        }
        blinks++;
        stones = new_stones;
    }
    
    u64 part1_answer = stones.size();
    u64 part2_answer = 0;


    fprintf(stdout, "2024: Day 10 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2024: Day 10 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
