// day_12.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


#include <string.h>


static const char* input_file_name = "day_12.test_input";  // Part 1: 21, part 2: 525152
// static const char* input_file_name = "day_12.input";  // Part 1: 7163, part 2:

static const u32 kMaxGroups = 50;
static const u32 kMaxPatternLength = 120;


struct Pattern
{
    u32 count;
    char springs[kMaxPatternLength];
};


struct Group
{
    u32 count;
    u32 groups[kMaxGroups];
};


u32 GenerateMatches(Pattern pattern, Group groups)
{
    // fprintf(stdout, "Trying %.*s\n", pattern.count, pattern.springs);
    
    u32 result = 0;
    
    bool done = false;
    
    for (int i = 0; i < pattern.count; ++i) {
        if (pattern.springs[i] == '?') {
            pattern.springs[i] = '.';
            int s1 = GenerateMatches(pattern, groups);
            pattern.springs[i] = '#';
            int s2 = GenerateMatches(pattern, groups);
            pattern.springs[i] = '?';
            result = s1 + s2;
            done = true;
            break;
        }
    }
    
    if (!done) {
        // fprintf(stdout, "    -> Comparing %.*s\n", pattern.count, pattern.springs);

        Group broken = {};
        bool last_was_broken = false;
        for (int i = 0; i < pattern.count; ++i) {
            if (pattern.springs[i] == '#') {
                last_was_broken = true;
                broken.groups[broken.count]++;
            } else {
                if (last_was_broken) {
                    last_was_broken = false;
                    broken.count++;
                }
            }
        }
        if (last_was_broken) {
            last_was_broken = false;
            broken.count++;
        }

        if (broken.count == groups.count) {
            bool all_match = true;
            for (int i = 0; i < broken.count; ++i) {
                if (broken.groups[i] != groups.groups[i]) {
                    all_match = false;
                    break;
                }
            }
            if (all_match) {
                result = 1;
            }
        }
    }
    
    return result;
}


void Day12()
{
    fprintf(stdout, "Day 12\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading file: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    i32 pattern_count = 0;
    u32 part_1_sum = 0;
    u64 part_2_sum = 0;
    u32 longest_pattern = 0;
    
    while (!AtEndOfFile(&parser)) {
        String pattern_string = ParseUntilSpace(&parser);
        fprintf(stdout, "%d: %.*s  ->  ", pattern_count, (int) pattern_string.size, pattern_string.start);
        Group group = {};
        while (!AtEndOfLine(&parser)) {
            group.groups[group.count] = ParseNumber(&parser);
            fprintf(stdout, "%u  ", group.groups[group.count]);
            group.count++;
        }

        Pattern pattern = {};
        memcpy(pattern.springs, pattern_string.start, pattern_string.size);
        pattern.count = (u32) pattern_string.size;

        u32 combinations = GenerateMatches(pattern, group);
        part_1_sum += combinations;
        fprintf(stdout, "(part 1 combos: %u, ", combinations);
        
        // Part 2 patterns and combos
        u32 offset = 0;
        Pattern expanded_pattern = {};
        for (int i = 0; i < 5; ++i) {
            memcpy(&expanded_pattern.springs[offset], pattern.springs, pattern.count);
            offset += pattern.count;
            expanded_pattern.springs[offset] = '?';
            offset++;
        }
        expanded_pattern.count = offset - 1;
        
        offset = 0;
        Group expanded_group = {};
        for (int i = 0; i < 5; ++i) {
            u32 bytes_to_copy = sizeof(u32) * group.count;
            memcpy(&expanded_group.groups[offset], group.groups, bytes_to_copy);
            offset += group.count;
        }
        expanded_group.count = group.count * 5;
        
        combinations = GenerateMatches(expanded_pattern, expanded_group);
        part_2_sum += combinations;
        fprintf(stdout, "part 2 combos: %u)\n", combinations);

        Advance(&parser);
        pattern_count++;
        if (pattern_string.size > longest_pattern) {
            longest_pattern = (u32) pattern_string.size;
        }
    }
    
    fprintf(stdout, "Longest pattern: %u\n", longest_pattern);
    
    fprintf(stdout, "Day 12 part 1 sum: %u\n", part_1_sum);
    fprintf(stdout, "Day 12 part 2 sum: %llu\n", part_2_sum);

    CloseFile(&input_file);
}
