// day_06.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_06.test_input";  // Part 1: 288, Part 2:
// static const char* input_file_name = "day_06.input";  // Part 1: 220320, Part 2:

static const int kMaxRaces = 4;
static const int kMaxPermutations = 100;


struct Race
{
    u32 time;
    u32 distance;
    u32 wins;
};


struct Trial
{
    u32 charge;
    u32 duration;
    u32 distance;
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
        // fprintf(stdout, "Race %d: time: %u, distance: %u\n", i, races[i].time, races[i].distance);
        Trial trials[kMaxPermutations] = {};
        for (int charge_time = 1; charge_time < (races[i].time - 1); ++charge_time) {
            Trial trial = {};
            trial.charge = charge_time;
            trial.duration = races[i].time - charge_time;
            trial.distance = charge_time * trial.duration;
            trials[charge_time] = trial;
            if (trial.distance > races[i].distance) {
                races[i].wins++;
            }
        }
    }
    
    u32 error = 1;
    for (int i = 0; i < race_count; ++i) {
        error *= races[i].wins;
    }
    
    fprintf(stdout, "Part 1: number: %u\n", error);
    
    CloseFile(&input_file);
}


