// day_11_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <unordered_map>
#include <vector>


// static const char* input_file_name = "../../2024/input/day_11.test_input";  // Part 1 = 55312, Part 2 =
static const char* input_file_name = "../../2024/input/day_11.input";  // Part 1 = 198075, Part 2 = 235571309320764


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


void AddStoneToMap(std::unordered_map<u64, u64>* stone_map, u64 stone, u64 count)
{
    auto it = stone_map->find(stone);
    if (it != stone_map->end()) {
        it->second += count;
    } else {
        (*stone_map)[stone] = count;
    }
}


u64 ComputeStoneCount(std::unordered_map<u64, u64>* map, u32 blinks)
{
    u64 result = 0;

    std::unordered_map<u64, u64> stone_map = *map;

    u32 current_blinks = 0;
    while (current_blinks < blinks) {
        std::unordered_map<u64, u64> new_stone_map;
        for (auto [stone, count] : stone_map) {
            if (stone == 0) {
                AddStoneToMap(&new_stone_map, 1, count);
            } else if (NumberOfDigits(stone) % 2 == 0) {
                Digits digits = SplitNumberInHalf(stone);
                AddStoneToMap(&new_stone_map, digits.left, count);
                AddStoneToMap(&new_stone_map, digits.right, count);
            } else {
                AddStoneToMap(&new_stone_map, stone * 2024, count);
            }
        }
        current_blinks++;
        stone_map = std::move(new_stone_map);
    }
    
    for (auto& [stone, count] : stone_map) {
        result += count;
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
    
    std::unordered_map<u64, u64> stone_map;
    while (!AtEndOfLine(&parser)) {
        u64 number = ParseNumber(&parser);
        auto it = stone_map.find(number);
        if (it != stone_map.end()) {
            it->second++;
        } else {
            stone_map[number] = 1;
        }
    }

    u64 part1_answer = ComputeStoneCount(&stone_map, 25);
    u64 part2_answer = ComputeStoneCount(&stone_map, 75);

    fprintf(stdout, "2024: Day 10 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2024: Day 10 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
