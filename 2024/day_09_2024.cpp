// day_09_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <vector>

// IDs: 10
// Files: 28
// Space: 14
// Total blocks: 42

// IDs: 10000
// Files: 49902
// Space: 45168
// Total blocks: 95070

// too high:
// 6353658456323

// static const char* input_file_name = "../../2024/input/day_09.test_input";  // Part 1 = 1928, Part 2 =
static const char* input_file_name = "../../2024/input/day_09.input";  // Part 1 = , Part 2 =


enum class BlockType : u16
{
    Space = 0,
    File
};


struct Block
{
    BlockType type;
    u16 id;
};


void AddBlocks(std::vector<Block>* blocks, u16 id, u16 count, BlockType type)
{
    Block block = { type, id };
    for (int i = 0; i < count; ++i) {
        blocks->push_back(block);
    }
}


u32 NextSpaceIndex(std::vector<Block>* blocks, i32 start_index)
{
    u32 result = blocks->size();
    
    while (start_index < blocks->size()) {
        if ((*blocks)[start_index].type == BlockType::Space) {
            result = start_index;
            break;
        }
        start_index++;
    }
    
    return result;
}


u32 LastFileIndex(std::vector<Block>* blocks, i32 start_index)
{
    u32 result = 0;
    
    while (start_index >= 0) {
        if ((*blocks)[start_index].type == BlockType::File) {
            result = start_index;
            break;
        }
        start_index--;
    }
    
    return result;
}


void PrintBlocks(const std::vector<Block>& blocks)
{
    for (int i = 0; i < blocks.size(); ++i) {
        if (blocks[i].type == BlockType::Space) {
            fprintf(stdout, ".");
        } else {
            fprintf(stdout, "%d", blocks[i].id);
        }
    }
    fprintf(stdout, "\n");
}


void Day09_2024()
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
    
    u32 id_count = 0;
    u32 space_count = 0;
    u32 file_count = 0;
    bool is_file = true;
    
    std::vector<Block> blocks;
    blocks.reserve(100000);
    
    while (!AtEndOfLine(&parser)) {
        u32 digit = (u32) (parser.data[parser.offset] - '0');
        if (is_file) {
            file_count += digit;
            AddBlocks(&blocks, id_count, digit, BlockType::File);
            id_count++;
        } else {
            space_count += digit;
            AddBlocks(&blocks, 0, digit, BlockType::Space);
        }
        is_file = !is_file;
        parser.offset++;
    }

    i32 space_index = 0;
    i32 file_index = blocks.size() - 1;
    
    // PrintBlocks(blocks);
    
    while (space_index < file_index) {
        space_index = NextSpaceIndex(&blocks, space_index);
        file_index = LastFileIndex(&blocks, file_index);
        assert(blocks[space_index].type == BlockType::Space);
        assert(blocks[file_index].type == BlockType::File);
        blocks[space_index] = blocks[file_index];
        blocks[file_index] = { BlockType::Space };
        space_index++;
        file_index--;
    }

    // PrintBlocks(blocks);
    
    for (int index = 0; index < blocks.size(); ++index) {
        if (blocks[index].type == BlockType::File) {
            part1_answer += index * blocks[index].id;
        }
    }


    fprintf(stdout, "IDs: %u\n", id_count);
    fprintf(stdout, "Files: %u\n", file_count);
    fprintf(stdout, "Space: %u\n", space_count);
    fprintf(stdout, "Total blocks: %u\n", file_count + space_count);

    fprintf(stdout, "2024: Day 09 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2024: Day 09 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
