// day_08.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

// static const char* input_file_name = "day_08.test_input";  // Part 1: 2
// static const char* input_file_name = "day_08.test_input2";  // Part 1: 6
// static const char* input_file_name = "day_08.test_input3";  // Part 2:
static const char* input_file_name = "day_08.input";  // Part 1: 19199, Part 2: 13663968099527

// Part 2 computes 14895418084045582289, need to find the least common multiple of all steps:
//    0: Steps: 13939
//    1: Steps: 17621
//    2: Steps: 11309
//    3: Steps: 20777
//    4: Steps: 19199
//    5: Steps: 15517
//
//    LCM: 13663968099527


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
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    u32 index_count = 0;
    IndexPair table[kMaxIndexValue] = {};
    
    u32 a_count = 0;
    u32 table_a[10] = {};
    u32 z_count = 0;
    u32 table_z[10] = {};
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    String directions = ParseWord(&parser);
    Advance(&parser, 2);
    
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
        
        if (index.start[2] == 'A') {
            table_a[a_count] = i;
            a_count++;
            // fprintf(stdout, "a: %.*s\n", (int) index.size, index.start);
        } else if (index.start[2] == 'Z') {
            table_z[z_count] = i;
            z_count++;
            // fprintf(stdout, "z: %.*s\n", (int) index.size, index.start);
        }
        
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

    u64 steps = 0;

    // Part 1: Steps from AAA to ZZZ.
    u32 current_index = StringToIndex(CONST_STRING("AAA"));
    u32 final_index = StringToIndex(CONST_STRING("ZZZ"));

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
            // fprintf(stdout, "Restarting directions\n");
        }
    }

    fprintf(stdout, "Part 1 steps: %llu\n", steps);

    // Part 2: All indices ending in 'A' to all ending in 'Z'.
    u64 steps_to_z[10] = {};
    
    for (int a = 0; a < a_count; ++a) {
        steps = 0;
        current_index = table_a[a];
        bool done = false;
        while (!done) {
            bool found_z = false;
            for (int i = 0; i < directions.size; ++i) {
                if (directions.start[i] == 'L') {
                    current_index = table[current_index].left;
                } else {
                    current_index = table[current_index].right;
                }
                steps++;
                for (int z = 0; z < a_count; ++z) {
                    if (current_index == table_z[z]) {
                        fprintf(stdout, "%d: Steps: %llu\n", a, steps);
                        steps_to_z[a] = steps;
                        found_z = true;
                        break;
                    }
                }
                if (found_z) {
                    done = true;
                    break;
                }
            }
        }
    }
    
    u64 steps_required = 1;
    for (int s = 0; s < a_count; ++s) {
        steps_required *= steps_to_z[s];
    }

    fprintf(stdout, "Part 2 steps: %llu\n", steps_required);
}


