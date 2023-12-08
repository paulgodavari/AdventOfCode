// day_08.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_08.test_input";  // Part 1: 2, Part 2:
// static const char* input_file_name = "day_08.test_input2";  // Part 1: 6, Part 2:
// static const char* input_file_name = "day_08.input";  // Part 1: 19199, Part 2:


static const u32 kMaxIndexValue = 26*26*26;


struct IndexPair
{
    u32 left;
    u32 right;
};


u32 StringToIndex(String str)
{
    u32 result = 0;
    
    for (int i = 0; i < str.size; ++i) {
        u32 num = str.start[i] - 'A';
        result = result * 26 + num;
    }
    
    return result;
}


void Day08()
{
    fprintf(stdout, "Day 08\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    IndexPair table[kMaxIndexValue] = {};
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    String directions = ParseWord(&parser);
    Advance(&parser, 2);
    
    u32 index_count = 0;
    while (!AtEndOfFile(&parser)) {
        String index = ParseWord(&parser);
        assert(index.size == 3);
        Advance(&parser, 4);
        String left = ParseWord(&parser);
        assert(left.size == 3);
        Advance(&parser, 2);
        String right = ParseWord(&parser);
        assert(right.size == 3);
        Advance(&parser);
        assert(AtEndOfLine(&parser));
        
        u32 i = StringToIndex(index);
        IndexPair p = { StringToIndex(left), StringToIndex(right) };
        assert(table[i].left == 0);
        assert(table[i].right == 0);
        table[i] = p;
        
//        fprintf(stdout, "%u: %.*s -> [%.*s, %.*s]",
//                index_count,
//                (int) index.size, index.start,
//                (int) left.size, left.start,
//                (int) right.size, right.start);
//        fprintf(stdout, "   (%u -> [%u, %u])", i, p.left, p.right);
//        fprintf(stdout, "\n");

        index_count++;
        Advance(&parser);
    }

    u32 current_index = StringToIndex(CONST_STRING("AAA"));
    u32 final_index = StringToIndex(CONST_STRING("ZZZ"));
    u32 steps = 0;
    
    bool done = false;
    while (!done) {
        for (int i = 0; i < directions.size; ++i) {
            if (directions.start[i] == 'L') {
                current_index = table[current_index].left;
            } else {
                assert(directions.start[i] == 'R');
                current_index = table[current_index].right;
            }
            steps++;
            if (current_index == final_index) {
                done = true;
                break;
            }
        }
        
        if (!done) {
            fprintf(stdout, "Restarting directions\n");
        }
    }
    
    fprintf(stdout, "Steps: %u\n", steps);
}


