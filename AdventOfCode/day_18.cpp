// day_18.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <vector>


static const char* input_file_name = "day_18.test_input";  // Part 1: , part 2:
// static const char* input_file_name = "day_18.input";  // Part 1: , part 2:


struct Color
{
    u32 r;
    u32 g;
    u32 b;
};


enum Direction
{
    kDirectionInvalid = 0,
    kDirectionUp,
    kDirectionDown,
    kDirectionLeft,
    kDirectionRight
};


struct Instruction
{
    Direction dir;
    u32 value;
    Color edge;
};


void Day18()
{
    fprintf(stdout, "Day 18\n");

    u64 run_time = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    fprintf(stdout, "Initialization time: %.4f\n", MillisecondsSince(run_time));
    
    // Part 1
    u32 part_1_sum = 0;
    u64 start_time = TimeNow();

    std::vector<Instruction> instructions;
    while (!AtEndOfFile(&parser)) {
        char dir;
        u32 value;
        u32 r, g, b;
        sscanf(&parser.data[parser.offset], "%c %u (#%2x%2x%2x)\n", &dir, &value, &r, &g, &b);
        Direction direction = kDirectionInvalid;
        switch (dir) {
            case 'U':
                direction = kDirectionUp;
                break;
            case 'D':
                direction = kDirectionDown;
                break;
            case 'L':
                direction = kDirectionLeft;
                break;
            case 'R':
                direction = kDirectionRight;
                break;
            default:
                break;
        }
        Instruction inst = { direction, value, { r, g, b} };
        instructions.push_back(inst);
        while (!AtEndOfLine(&parser)) {
            Advance(&parser);
        }
        Advance(&parser);
    }
    
    fprintf(stdout, "Parse time: %.4f ms\n", MillisecondsSince(start_time));
    start_time = TimeNow();

    fprintf(stdout, "Part 1 sum: %u (%.4f ms)\n", part_1_sum, MillisecondsSince(start_time));

    fprintf(stdout, "Run time: %.4f ms\n", MillisecondsSince(run_time));
    
    CloseFile(&input_file);
}


