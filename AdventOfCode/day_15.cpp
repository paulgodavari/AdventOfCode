// day_15.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_15.test_input";  // Part 1: 1320, part2:
// static const char* input_file_name = "day_15.input";  // Part 1: 512797, part2:


u32 ComputeHash(String word)
{
    u32 result = 0;
    
    for (int i = 0; i < word.size; ++i) {
        result += (u32) word.start[i];
        result *= 17;
        result = result % 256;
    }
    
    return result;
}


void Day15()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    fprintf(stdout, "Day 15\n");
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    u32 part_1_sum = 0;
    size_t word_length = 0;
    const char* word_start = nullptr;

    while (!AtEndOfFile(&parser)) {
        char c = parser.data[parser.offset];
        switch (c) {
            case ',':
            case '\n': {
                String word = { word_start, word_length };
                u32 hash = ComputeHash(word);
                fprintf(stdout, "Found word: %.*s -> %u\n", (int) word.size, word.start, hash);
                part_1_sum += hash;
                word_start = nullptr;
                word_length = 0;
                break;
            }
            default: {
                if (!word_start) {
                    word_start = parser.data + parser.offset;
                }
                word_length++;
                break;
            }
        }
        Advance(&parser);
    }
    
    
    fprintf(stdout, "Day 15: part 1 sum: %u\n", part_1_sum);
    
    CloseFile(&input_file);
}


