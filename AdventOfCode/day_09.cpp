// day_09.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


// static const char* input_file_name = "day_09.test_input";  // Part 1: 114, part 2:
static const char* input_file_name = "day_09.input";  // Part 1: 1819125966, part 2:

static const int kMaxSamples = 100;
static const int kMaxRows = 200;


struct History
{
    u32 count;
    i32 samples[kMaxSamples];
};


i32 PredictNextValue(History* history)
{
    i32 result = 0;
    
    // fprintf(stdout, "    ");
    History diffs = {};
    u32 zero_count = 0;
    for (int i = 1; i < history->count; ++i) {
        i32 diff = history->samples[i] - history->samples[i-1];
        diffs.samples[diffs.count] = diff;
        diffs.count++;
        if (diff == 0) {
            zero_count++;
        }
        // fprintf(stdout, "%d, ", diff);        
    }
    // fprintf(stdout, "    \n");
    
    if (zero_count == diffs.count) {
        // We're done, return something.
        // fprintf(stdout, " -> %d\n", result);
        return 0;
    } else {
        result = PredictNextValue(&diffs);
    }
    
    // result += history->samples[history->count-1];
    result += diffs.samples[diffs.count-1];
    // fprintf(stdout, " -> %d\n", result);
    
    return result;
}


void Day09()
{
    fprintf(stdout, "Day 09\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    History table[kMaxRows] = {};
    
    u32 line_count = 0;
    while (!AtEndOfFile(&parser)) {
        History* history = &table[line_count];
        while (!AtEndOfLine(&parser)) {
            history->samples[history->count] = ParseSignedNumber(&parser);
            history->count++;
        }
        Advance(&parser);
        line_count++;
    }

    i64 prediction_sum = 0;
    
    for (int row = 0; row < line_count; ++row) {
        // fprintf(stdout, "%u: ", row);
        History* history = &table[row];
        for (int i = 0; i < history->count; ++i) {
            // fprintf(stdout, "%u, ", history->samples[i]);
        }
        // fprintf(stdout, "\n");
        i32 prediction = PredictNextValue(history);
        prediction_sum += prediction + history->samples[history->count-1];
    }
    
    fprintf(stdout, "Prediction sum: %lld\n", prediction_sum);
}


