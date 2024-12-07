// day_07_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 

#include "advent_of_code.h"

#include <vector>


// static const char* input_file_name = "../../2024/input/day_07.test_input";  // Part 1 = 3749, Part 2 = 
static const char* input_file_name = "../../2024/input/day_07.input";  // Part 1 = 12839601725877, Part 2 =


struct State
{
    const std::vector<u64>* numbers;
    u64 expected_value;
    u32 match_count;    
};


enum class Operation
{
    Invalid = 0,
    Add,
    Multiply
};


void ComputeValue(State* state, u64 value, Operation op, u32 index)
{
    assert(op == Operation::Add || op == Operation::Multiply);
    
    u64 current_value = (*state->numbers)[index];
    value = (op == Operation::Add) ? (value + current_value) : (value * current_value);
    
    if (index == state->numbers->size() - 1) {
        if (value == state->expected_value) {
            state->match_count += 1;
        }
    } else {
        ComputeValue(state, value, Operation::Add, index + 1);
        ComputeValue(state, value, Operation::Multiply, index + 1);
    }
}


void Day07_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    u64 part1_answer = 0;
    u64 part2_answer = 0;
    
    while (!AtEndOfFile(&parser)) {
        u64 value = ParseNumber(&parser);
        assert(parser.data[parser.offset] == ':');
        Advance(&parser);
        std::vector<u64> numbers;
        while (!AtEndOfLine(&parser)) {
            u64 number = ParseNumber(&parser);
            numbers.push_back(number);
        }
        Advance(&parser);
        
        State state = { &numbers, value, 0 };
        
        u32 index = 0;
        ComputeValue(&state, numbers[index], Operation::Add, index + 1);
        ComputeValue(&state, numbers[index], Operation::Multiply, index + 1);
        
        if (state.match_count > 0) {
            part1_answer += value;
        }
    }

    fprintf(stdout, "2024: Day 04 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2024: Day 04 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
