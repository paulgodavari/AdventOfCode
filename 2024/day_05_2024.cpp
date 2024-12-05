// day_05_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <bitset>
#include <unordered_map>
#include <vector>


static const char* input_file_name = "../../2024/input/day_05.test_input";  // Part 1 = 143, Part 2 = 123
// static const char* input_file_name = "../../2024/input/day_05.input";  // Part 1 = 6498, Part 2 =


void Day05_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    u32 part1_answer = 0;
    
    std::unordered_map<u32, std::bitset<100>> rules;
    
    while (!AtEndOfFile(&parser) && parser.data[parser.offset] != '\n') {
        // Parse the rules
        u32 page = ParseNumber(&parser);
        assert(parser.data[parser.offset] == '|');
        Advance(&parser);
        u32 previous_page = ParseNumber(&parser);
        assert(parser.data[parser.offset] == '\n');
        Advance(&parser);
        
        // Update the rules.
        auto it = rules.find(page);
        if (it == rules.end()) {
            std::bitset<100> previous_pages;
            previous_pages.set(previous_page);
            rules[page] = previous_pages;
        } else {
            it->second.set(previous_page);
        }
    }
    
    Advance(&parser);
    
    // Parse the page list
    while (!AtEndOfFile(&parser)) {
        std::vector<u32> pages;
        bool correct = true;
        while (!AtEndOfLine(&parser)) {
            u32 page = ParseNumber(&parser);
            
            auto it = rules.find(page);
            if (it != rules.end()) {
                for (int index = 0; index < pages.size(); ++index) {
                    u32 seen_page = pages[index];
                    if (it->second[seen_page]) {
                        correct = false;
                    }
                }
            }

            pages.push_back(page);

            if (parser.data[parser.offset] == ',') {
                Advance(&parser);
            }
        }
        
        Advance(&parser);
        
        if (correct) {
            int index = pages.size() / 2;
            part1_answer += pages[index];
        }
    }
    
    
    u32 part2_answer = 0;

    fprintf(stdout, "2024: Day 04 part 1: %u\n", part1_answer);
    fprintf(stdout, "2024: Day 04 part 2: %u\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
