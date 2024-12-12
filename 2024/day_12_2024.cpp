// day_12_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>


static const char* input_file_name = "../../2024/input/day_12.test_input";  // Part 1 = 1930, Part 2 = 1206
// static const char* input_file_name = "../../2024/input/day_12.input";  // Part 1 = 1371306, Part 2 =


enum class Direction
{
    Invalid = 0,
    Up,
    Right,
    Down,
    Left
};


struct Grid
{
    ParseState* parser;
    i32 rows;
    i32 cols;  // Includes newline
};


struct Position
{
    i32 row;
    i32 col;
    
    bool operator==(const Position& other) const {
        return col == other.col && row == other.row;
    }
};


namespace std {
    template <>
    struct hash<Position> {
        std::size_t operator()(const Position& p) const {
            return std::hash<int>()(p.col) ^ (std::hash<int>()(p.row) << 1);
        }
    };
}


struct Move
{
    Position position;
    Direction direction;
    
    bool operator==(const Move& other) {
        return position == other.position && direction == other.direction;
    }
};


static Grid ComputeGrid(ParseState* parser)
{
    Grid result = { parser, 0, 0 };
    
    while (parser->data[result.cols] != '\n') {
        result.cols++;
    }
    result.cols++;  // Include the newline in calculations.
    result.rows = (i32) parser->size / result.cols;

    return result;
}


static u32 OffsetFromPosition(Grid* grid, Position position)
{
    u32 result = position.row * grid->cols + position.col;
    return result;
}


static bool IsValidPosition(Grid* grid, Position position)
{
    return (position.col >= 0 && position.col < grid->cols - 1) && (position.row >= 0 && position.row < grid->rows);
}


static char ValueAtPosition(Grid* grid, Position position)
{
    char result = -1;
    
    u32 offset = OffsetFromPosition(grid, position);
    if (offset < grid->parser->size) {
        result = grid->parser->data[offset];
    }
    
    return result;
}


void FindRegion(Grid* grid, Position position, u32 region_id,
                std::unordered_set<Position>* visited_positions,
                std::unordered_map<u32, std::vector<Position>>* region_map)
{
    char region_char = ValueAtPosition(grid, position);
    
    // Mark our current position as visited.
    assert(visited_positions->find(position) == visited_positions->end());
    visited_positions->insert(position);
    
    // Add our current position to the current region.
    auto it = region_map->find(region_id);
    if (it != region_map->end()) {
        it->second.push_back(position);
    } else {
        std::vector<Position> positions;
        positions.push_back(position);
        (*region_map)[region_id] = positions;
    }
    
    // Check the surrounding plots: up, right, down, left.
    Position up = { position.row - 1, position.col };
    if (IsValidPosition(grid, up) && ValueAtPosition(grid, up) == region_char) {
        if (visited_positions->find(up) == visited_positions->end()) {
            FindRegion(grid, up, region_id, visited_positions, region_map);
        }
    }

    Position right = { position.row, position.col + 1 };
    if (IsValidPosition(grid, right) && ValueAtPosition(grid, right) == region_char) {
        if (visited_positions->find(right) == visited_positions->end()) {
            FindRegion(grid, right, region_id, visited_positions, region_map);
        }
    }

    Position down = { position.row + 1, position.col };
    if (IsValidPosition(grid, down) && ValueAtPosition(grid, down) == region_char) {
        if (visited_positions->find(down) == visited_positions->end()) {
            FindRegion(grid, down, region_id, visited_positions, region_map);
        }
    }

    Position left = { position.row, position.col - 1 };
    if (IsValidPosition(grid, left) && ValueAtPosition(grid, left) == region_char) {
        if (visited_positions->find(left) == visited_positions->end()) {
            FindRegion(grid, left, region_id, visited_positions, region_map);
        }
    }
}


u64 ComputePerimeterAtPosition(Grid* grid, Position position)
{
    u64 result = 0;
    
    char region_char = ValueAtPosition(grid, position);
    
    Position up = { position.row - 1, position.col };
    if (!IsValidPosition(grid, up)) {
        result += 1;
    } else {
        if (ValueAtPosition(grid, up) != region_char) {
            result += 1;
        }
    }
    
    Position right = { position.row, position.col + 1 };
    if (!IsValidPosition(grid, right)) {
        result += 1;
    } else {
        if (ValueAtPosition(grid, right) != region_char) {
            result += 1;
        }
    }
    
    Position down = { position.row + 1, position.col };
    if (!IsValidPosition(grid, down)) {
        result += 1;
    } else {
        if (ValueAtPosition(grid, down) != region_char) {
            result += 1;
        }
    }
    
    Position left = { position.row, position.col - 1 };
    if (!IsValidPosition(grid, left)) {
        result += 1;
    } else {
        if (ValueAtPosition(grid, left) != region_char) {
            result += 1;
        }
    }
    
    return result;
}


u64 ComputePerimeter(Grid* grid, const std::vector<Position>& positions)
{
    u64 result = 0;
    
    for (i32 index = 0; index < positions.size(); ++index) {
        result += ComputePerimeterAtPosition(grid, positions[index]);
    }
    
    return result;
}


bool HasBorderForDirection(Grid* grid, Position position, Direction direction)
{
    bool result = false;
    
    Position new_position = position;
    switch (direction) {
        case Direction::Up:
            new_position.row -= 1;
            break;
        case Direction::Right:
            new_position.col += 1;
            break;
        case Direction::Down:
            new_position.row += 1;
            break;
        case Direction::Left:
            new_position.col -= 1;
            break;
        default:
            assert(0);
            break;
    }
    
    if (IsValidPosition(grid, new_position)) {
        char current = ValueAtPosition(grid, position);
        char next = ValueAtPosition(grid, new_position);
        if (current != next) {
            result = true;
        }
    } else {
        result = true;
    }
    
    return result;
}


Move GetNextMove(Grid* grid, Move previous_move)
{
    Move result = {};
    
    switch (previous_move.direction) {
        case Direction::Up: {
            // See if we can keep going up.
            Position test = { previous_move.position.row - 1, previous_move.position.col };
            if (IsValidPosition(grid, test)) {
            } else {
                // Going up, but no valid position above so we have a border.
                result.position = previous_move;
                result.direction = Direction::Right;
            }
            break;
        }
    }
    
    return result;
}


u64 ComputeSides(Grid* grid, const std::vector<Position>& positions)
{
    u64 result = 0;
    
    // Find the first position with a border.
    Move start = { { -1, -1 }, Direction::Invalid };
    
    for (i32 index = 0; index < positions.size(); ++index) {
        Position position = positions[index];
        if (HasBorderForDirection(grid, position, Direction::Up)) {
            start.position = position;
            start.direction = Direction::Right;
            break;
        }
        if (HasBorderForDirection(grid, position, Direction::Right)) {
            start.position = position;
            start.direction = Direction::Down;
            break;
        }
        if (HasBorderForDirection(grid, position, Direction::Down)) {
            start.position = position;
            start.direction = Direction::Left;
            break;
        }
        if (HasBorderForDirection(grid, position, Direction::Left)) {
            start.position = position;
            start.direction = Direction::Up;
            break;
        }
    }
    
    assert(IsValidPosition(grid, start.position));
    assert(start.direction != Direction::Invalid);
    
    Move current_move = start;
    Move next_move = GetNextMove(grid, start);
    
    while (next_move != start) {
        if (next_move.direction != current_move.direction) {
            // Each change in direction is a new corner, number of corners == number of sides.
            result++;
        }
        current_move = next_move;
        next_move = GetNextMove(grid, current_move);
    }
    
    return result;
}


void Day12_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    Grid grid = ComputeGrid(&parser);
    
    u32 region_id = 0;
    std::unordered_set<Position> visited_positions;
    std::unordered_map<u32, std::vector<Position>> region_map;
    
    u64 part1_answer = 0;
    u64 part2_answer = 0;

    // Discover all the regions
    for (i32 row = 0; row < grid.rows; ++row) {
        for (i32 col = 0; col < grid.cols - 1; ++col) {
            Position position = { row, col };
            if (visited_positions.find(position) == visited_positions.end()) {
                FindRegion(&grid, position, region_id, &visited_positions, &region_map);
                region_id++;
            }
        }
    }
    
    // Part 1: Compute the perimeter for each region.
    // Part 2: Compute the number of sides of each region.
    for (auto& [rid, positions] : region_map) {
        // fprintf(stdout, "Region %u: size: %zu\n", rid, positions.size());
        u64 perimeter = ComputePerimeter(&grid, positions);
        part1_answer += perimeter * positions.size();
        u64 sides = ComputeSides(&grid, positions);
        part2_answer += sides * positions.size();
    }
    
    fprintf(stdout, "2024: Day 10 part 1: %llu\n", part1_answer);
    fprintf(stdout, "2024: Day 10 part 2: %llu\n", part2_answer);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}
