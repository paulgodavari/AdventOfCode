// day_06.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_06.test_input";
// static const char* input_file_name = "day_06.input";

static const int kMaxRaces = 4;


struct Race
{
    u32 time;
    u32 distance;
    u32 wins;
};


void Day06()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error opening file: %s\n", input_file_name);
        return;
    }
    ParseState parser = { input_file.data, input_file.size, 0 };

    int race_count = 0;
    int index = 0;
    Race races[kMaxRaces] = {};

    // Parse the input data.
    ConsumeString(&parser, CONST_STRING("Time:"));
    while (!AtEndOfLine(&parser)) {
        races[index].time = ParseNumber(&parser);
        index++;
        race_count++;
    }
    
    Advance(&parser);
    
    ConsumeString(&parser, CONST_STRING("Distance:"));
    index = 0;
    while (!AtEndOfLine(&parser)) {
        races[index].distance = ParseNumber(&parser);
        index++;
    }

    for (int i = 0; i < race_count; ++i) {
        fprintf(stdout, "Race %d: time: %u, distance: %u\n", i, races[i].time, races[i].distance);
    }
    
    CloseFile(&input_file);
}


