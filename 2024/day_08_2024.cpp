// day_08_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


#include <vector>
#include <unordered_map>


// static const char* input_file_name = "../../2024/input/day_08.test_input";  // Part 1 = 14, Part 2 =
static const char* input_file_name = "../../2024/input/day_08.input";  // Part 1 = 381, Part 2 =


struct Position
{
    i32 col;
    i32 row;
    
    bool operator==(const Position& other) const {
        return col == other.col && row == other.row;
    }
};


namespace std {
    template <>
    struct hash<Position> {
        std::size_t operator()(const Position& p) const {
            // Combine the two integers into a unique hash
            return std::hash<int>()(p.col) ^ (std::hash<int>()(p.row) << 1);
        }
    };
}


Position operator-(const Position& lhs, const Position& rhs)
{
    Position result = { lhs.col - rhs.col, lhs.row - rhs.row };
    return result;
}


Position operator+(const Position& lhs, const Position& rhs)
{
    Position result = { lhs.col + rhs.col, lhs.row + rhs.row };
    return result;
}


// Final column in every row is '\n' which we need for row/col calculation,
// but is considered an invalid position.

struct Grid
{
    ParseState* parser;
    i32 rows;
    i32 cols;
};


Grid ComputeGrid(ParseState* parser)
{
    Grid result = { parser, 0, 0 };
    
    while (parser->data[result.cols] != '\n') {
        result.cols++;
    }
    result.cols++;  // Include the newline in our calculations
    result.rows = (i32) parser->size / result.cols;
    
    return result;
}


Position OffsetToGridPosition(Grid* grid, u32 offset)
{
    Position result = {};
    
    result.row = offset / grid->cols;
    result.col = offset % grid->cols;
    
    return result;
}


bool IsValidPosition(Grid* grid, Position position)
{
    return (position.col >= 0 && position.col < grid->cols - 1) && (position.row >= 0 && position.row < grid->rows);
}


void Day08_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    Grid grid = ComputeGrid(&parser);
    
    // Part 1: Find all the frequencies and their positions in the grid.
    std::unordered_map<char, std::vector<Position>> frequencies;
    while (!AtEndOfFile(&parser)) {
        char current = parser.data[parser.offset];
        Position position = OffsetToGridPosition(&grid, parser.offset);
        if (current != '\n' && current != '.') {
            assert(IsValidPosition(&grid, position));
            auto it = frequencies.find(current);
            if (it != frequencies.end()) {
                it->second.push_back(position);
            } else {
                std::vector<Position> positions;
                positions.push_back(position);
                frequencies[current] = positions;
            }
        }
        Advance(&parser);
    }
    
    // Part 1: For each frequency, compute the antinodes for each pair of positions.
    std::unordered_map<Position, u32> position_map;
    for (auto& [frequency, positions] : frequencies) {
        if (positions.size() > 1) {
            for (int i = 0; i < positions.size() - 1; ++i) {
                for (int j = i + 1; j < positions.size(); ++j) {
                    Position diff = positions[i] - positions[j];
                    Position antinode1 = positions[i] + diff;
                    Position antinode2 = positions[j] - diff;
                    if (IsValidPosition(&grid, antinode1)) {
                        auto it = position_map.find(antinode1);
                        if (it != position_map.end()) {
                            it->second++;
                        } else {
                            position_map[antinode1] = 1;
                        }
                    }
                    if (IsValidPosition(&grid, antinode2)) {
                        auto it = position_map.find(antinode2);
                        if (it != position_map.end()) {
                            it->second++;
                        } else {
                            position_map[antinode2] = 1;
                        }
                    }
                }
            }
        }
        //fprintf(stdout, "Freq: %c -> ", frequency);
        //for (int index = 0; index < positions.size(); ++index) {
        //    fprintf(stdout, "(%d, %d) ", positions[index].col, positions[index].row);
        //}
        //fprintf(stdout, "\n");
    }
    
    u64 part1_answer = position_map.size();
    
    
    u64 part2_answer = 0;
    
    fprintf(stdout, "2024: Day 08 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2024: Day 08 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
