// day_15.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


// static const char* input_file_name = "day_15.test_input";  // Part 1: 1320, part2: 145
static const char* input_file_name = "day_15.input";  // Part 1: 512797, part2: 262454

static const u32 kMaxBoxes = 256;


enum Operation
{
    kCommandInvalid = 0,
    kCommandDash,
    kCommandEquals
};


struct Command
{
    String label;
    Operation op;
    u32 value;
};


struct HashBucket
{
    HashBucket* next;
    String label;
    u32 value;
};


struct HashMap
{
    HashBucket* box[kMaxBoxes];
};


u32 ComputeHash(String word)
{
    u32 result = 0;
    
    for (int i = 0; i < word.size; ++i) {
        result += (u32) word.start[i];
        result *= 17;
        result = result % 256;
    }
    
    return result;
}


Command ParseCommand(String op_code)
{
    Command result = {};
    
    ParseState parser = { (char*)op_code.start, op_code.size, 0 };
    
    bool parsing_label = true;
    while (parser.offset < parser.size) {
        char c = parser.data[parser.offset];
        switch (c) {
            case '=': {
                result.op = kCommandEquals;
                parsing_label = false;
                Advance(&parser);
                break;
            }
            case '-': {
                result.op = kCommandDash;
                parsing_label = false;
                Advance(&parser);
                break;
            }
            default: {
                if (parsing_label) {
                    result.label = ParseWord(&parser);
                } else {
                    result.value = ParseNumber(&parser);
                }
                break;
            }
        }
    }
    
    return result;
}


void ProcessCommand(HashMap* hash_map, Command cmd)
{
    u32 box_index = ComputeHash(cmd.label);
    
    switch (cmd.op) {
        case kCommandDash: {
            if (hash_map->box[box_index]) {
                HashBucket* next = hash_map->box[box_index];
                HashBucket* current = nullptr;
                while (next) {
                    if (cmd.label == next->label) {
                        if (current) {
                            current->next = next->next;
                        } else {
                            hash_map->box[box_index] = next->next;
                        }
                        delete next;
                        break;
                    }                    
                    current = next;
                    next = next->next;
                }
            }
            break;
        }
        case kCommandEquals: {
            if (!hash_map->box[box_index]) {
                hash_map->box[box_index] = new HashBucket { nullptr, cmd.label, cmd.value };
            } else {
                // Search for an existing entry with the same label.
                bool found_entry = false;
                HashBucket* next = hash_map->box[box_index];
                HashBucket* current = next;
                while (next) {
                    if (cmd.label == next->label) {
                        found_entry = true;
                        next->value = cmd.value;
                        break;
                    }
                    current = next;
                    next = next->next;
                }
                if (!found_entry) {
                    current->next = new HashBucket { nullptr, cmd.label, cmd.value };
                }
            }
            break;
        }
        default: {
            break;
        }
    }
    
}


u32 ScoreHashMap(HashMap* hash_map)
{
    u32 result = 0;
    
    for (int i = 0; i < kMaxBoxes; ++i) {
        u32 box_sum = 0;
        u32 box_score = i + 1;
        if (hash_map->box[i]) {
            HashBucket* next = hash_map->box[i];
            u32 bucket_count = 1;
            while (next) {
                box_sum += box_score * bucket_count * next->value;
                next = next->next;
                bucket_count++;
            }
        }
        result += box_sum;
    }
    
    return result;
}


void DumpHashMap(HashMap* hash_map)
{
    fprintf(stdout, "\n");
    
    for (int i = 0; i < kMaxBoxes; ++i) {
        if (hash_map->box[i]) {
            fprintf(stdout, "Box %d: ", i);
            HashBucket* next = hash_map->box[i];
            while (next) {
                fprintf(stdout, "[%.*s %u] ", (int) next->label.size, next->label.start, next->value);
                next = next->next;
            }
            fprintf(stdout, "\n");
        }
    }
    
    fprintf(stdout, "\n");
}


void Day15()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    fprintf(stdout, "Day 15\n");
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    u32 part_1_sum = 0;
    size_t word_length = 0;
    const char* word_start = nullptr;
    
    HashMap hash_map = {};

    while (!AtEndOfFile(&parser)) {
        char c = parser.data[parser.offset];
        switch (c) {
            case ',':
            case '\n': {
                String op_code = { word_start, word_length };
                
                // Part 1
                u32 hash = ComputeHash(op_code);
                // fprintf(stdout, "Found word: %.*s -> %u\n", (int) op_code.size, op_code.start, hash);
                part_1_sum += hash;
                word_start = nullptr;
                word_length = 0;
                
                // Part 2
                Command cmd = ParseCommand(op_code);
                // fprintf(stdout, "Command label: %.*s, op: %u, value: %u\n",
                //         (int) cmd.label.size, cmd.label.start, cmd.op, cmd.value);
                ProcessCommand(&hash_map, cmd);
                // DumpHashMap(&hash_map);
                break;
            }
            default: {
                if (!word_start) {
                    word_start = parser.data + parser.offset;
                }
                word_length++;
                break;
            }
        }
        Advance(&parser);
    }

    u32 part_2_sum = ScoreHashMap(&hash_map);
    
    fprintf(stdout, "Day 15: part 1 sum: %u\n", part_1_sum);
    fprintf(stdout, "Day 15: part 2 sum: %u\n", part_2_sum);
        
    CloseFile(&input_file);
}


