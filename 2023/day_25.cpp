// day_25.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/advent_of_code.h"

#include <string.h>
#include <vector>


struct WiringNode
{
    String name;
    std::vector<String> edges;
};


// The graph parser is in place for the unfinished solver.
[[maybe_unused]] static bool ReadWiring(const File* input_file, std::vector<WiringNode>* nodes)
{
    String input = { input_file->data, input_file->size };
    while (input.size) {
        String line = ReadLine(&input);
        const char* colon = (const char*) memchr(line.start, ':', line.size);
        if (!colon || colon == line.start) {
            return false;
        }

        size_t name_size = (size_t) (colon - line.start);
        WiringNode node = { { line.start, name_size }, {} };
        for (size_t index = name_size + 1; index < line.size;) {
            while (index < line.size && line.start[index] == ' ') {
                ++index;
            }
            size_t start = index;
            while (index < line.size && line.start[index] != ' ') {
                ++index;
            }
            if (index != start) {
                node.edges.push_back({ line.start + start, index - start });
            }
        }
        nodes->push_back(std::move(node));
    }
    return true;
}
