// day_05_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <bitset>
#include <unordered_map>
#include <vector>


// static const char* input_file_name = "../../2024/input/day_05.test_input";  // Part 1 = 143, Part 2 = 123
static const char* input_file_name = "../../2024/input/day_05.input";  // Part 1 = 6498, Part 2 = 5017


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
    u32 part2_answer = 0;

    std::unordered_map<u32, std::bitset<100>> rules;
    
    // Parse the rules
    while (!AtEndOfFile(&parser) && parser.data[parser.offset] != '\n') {
        u32 page = (u32) ParseNumber(&parser);
        assert(parser.data[parser.offset] == '|');
        Advance(&parser);
        u32 previous_page = (u32) ParseNumber(&parser);
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

    // Move beyond the blank separator line
    Advance(&parser);

    // Parse the page list
    while (!AtEndOfFile(&parser)) {
        std::vector<u32> pages;
        bool correct = true;
        while (!AtEndOfLine(&parser)) {
            u32 page = (u32) ParseNumber(&parser);
            
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
            // Part 1: Sum correct pages middle page
            u32 index = (u32) pages.size() / 2;
            part1_answer += pages[index];
        } else {
            // Part 2: Fix incorrect pages sort order, then sum middle page.
            //
            // - Choose the first item in pages
            // - Check if it is in any the other page's rules
            //     - Yes: item can't be next, go to the next item in pages
            //     - No: item can be next, put the item in sorted_pages and remove it from pages
            // - Back to start (with a shrinking pages vector)
            
            std::vector<u32> sorted_pages;
            
            u32 index = 0;
            
            while (pages.size() > 0) {
                bool is_first = true;
                u32 page = pages[index];
                for (int i = 0; i < pages.size(); ++i) {
                    if (i != index) {
                        auto it = rules.find(pages[i]);
                        if (it != rules.end() && it->second[page]) {
                            is_first = false;
                            break;
                        }
                    }
                }
                if (is_first) {
                    sorted_pages.push_back(page);
                    pages.erase(pages.begin() + index);
                    index = 0;
                } else {
                    index++;
                }
            }
            
            u32 middle_index = (u32) sorted_pages.size() / 2;
            part2_answer += sorted_pages[middle_index];
        }
    }

    fprintf(stdout, "2024: Day 05 part 1: %u\n", part1_answer);
    fprintf(stdout, "2024: Day 05 part 2: %u\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
