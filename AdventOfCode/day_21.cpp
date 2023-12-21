// day_21.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


//static const char* input_file_name = "day_21.test_input";  // Part 1: 16, part 2:
static const u32 kMaxSteps = 6;

static const char* input_file_name = "day_21.input";  // Part 1: , part 2:
// static const u32 kMaxSteps = 64;

static const u32 kMaxRows = 131;
static const u32 kMaxCols = 131;


struct Position
{
    i32 row;
    i32 col;
};


void Day21()
{
    fprintf(stdout, "Day 21\n");

    u64 run_time = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    fprintf(stdout, "Initialization time: %.4f\n", MillisecondsSince(run_time));
    
    // Part 1
    u64 start_time = TimeNow();
    
    // Find grid size and starting position.

    Position dimensions = {};
    Position start_pos = {};
    Position current_pos = {};
    while (!AtEndOfFile(&parser)) {
        current_pos.col = 0;
        while (!AtEndOfLine(&parser)) {
            char current = parser.data[parser.offset];
            if (current == 'S') {
                start_pos = current_pos;
            }
            Advance(&parser);
            current_pos.col++;
        }
        Advance(&parser);
        current_pos.col++;
        current_pos.row++;
        if (dimensions.col < current_pos.col) {
            dimensions.col = current_pos.col;
        }
    }
    
    dimensions.row = current_pos.row;
    fprintf(stdout, "Start position: r: %u, c: %u\n", start_pos.row, start_pos.col);
    fprintf(stdout, "Dimensions: r: %u, c: %u\n", dimensions.row, dimensions.col);

    fprintf(stdout, "Run time: %.4f ms\n", MillisecondsSince(run_time));
    
    CloseFile(&input_file);
}


