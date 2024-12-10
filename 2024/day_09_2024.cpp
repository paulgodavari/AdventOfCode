// day_09_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <vector>

// static const char* input_file_name = "../../2024/input/day_09.test_input";  // Part 1 = 1928, Part 2 = 2858
static const char* input_file_name = "../../2024/input/day_09.input";  // Part 1 = 6353658451014, Part 2 = 6382582136592


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


struct Span
{
    BlockType type;
    u16 id;
    u32 index;
    u32 size;
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
            fprintf(stdout, "%x", blocks[i].id);
        }
    }
    fprintf(stdout, "\n");
}


void PrintSpan(const std::vector<Span>& spans)
{
    for (int i = 0; i < spans.size(); ++i) {
        Span span = spans[i];
        if (span.type == BlockType::Space) {
            for (int j = 0; j < span.size; ++j) {
                fprintf(stdout, ".");
            }
        } else {
            for (int j = 0; j < span.size; ++j) {
                fprintf(stdout, "%x", span.id);
            }
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
    i32 file_index = (i32) blocks.size() - 1;
    
    // PrintBlocks(blocks);
    
    while (space_index < file_index) {
        space_index = NextSpaceIndex(&blocks, space_index);
        file_index = LastFileIndex(&blocks, file_index);
        if (space_index < file_index) {
            assert(blocks[space_index].type == BlockType::Space);
            assert(blocks[file_index].type == BlockType::File);
            blocks[space_index] = blocks[file_index];
            blocks[file_index] = { BlockType::Space };
            space_index++;
            file_index--;
        }
    }

    // PrintBlocks(blocks);
    
    for (int index = 0; index < blocks.size(); ++index) {
        if (blocks[index].type == BlockType::File) {
            part1_answer += index * blocks[index].id;
        }
    }
    
    // Part 2. Reset everything and re-parse.
    parser.offset = 0;
    is_file = true;
    id_count = 0;

    u32 index = 0;
    std::vector<Span> files;
    std::vector<Span> spaces;

    while (!AtEndOfLine(&parser)) {
        u32 digit = (u32) (parser.data[parser.offset] - '0');
        if (is_file) {
            Span span = { BlockType::File, (u16) id_count, index, digit };
            files.push_back(span);
            id_count++;
        } else {
            Span span = { BlockType::Space, 0, index, digit };
            spaces.push_back(span);
        }
        is_file = !is_file;
        parser.offset++;
        index += digit;
    }

    // PrintSpan(spans);
    
    // Try to find the first that fits for each file, last file first.
    for (int i = files.size() - 1; i > 0; --i) {
        Span& file = files[i];
        for (int j = 0; j < spaces.size(); ++j) {
            Span& space = spaces[j];
            if (space.index < file.index && space.size >= file.size) {
                // We can move the file into the space.
                file.index = space.index;
                space.size -= file.size;
                space.index += file.size;
                break;
            }
        }
    }
    
    // Compute checksum
    u64 part2_answer = 0;
    for (int i = 0; i < files.size(); ++i) {
        Span file = files[i];
        for (int j = 0; j < file.size; ++j) {
            part2_answer += file.id * (file.index + j);
        }
    }

    fprintf(stdout, "2024: Day 09 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2024: Day 09 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
