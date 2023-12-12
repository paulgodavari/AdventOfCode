// day_11.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_11.test_input";  // Part 1: 374, part 2:
// static const char* input_file_name = "day_11.input";  // Part 1: , part 2:


static const u32 kMaxGalaxies = 512;
static const u32 kMaxRows = 140;
static const u32 kMaxCols = 140;  // 141 if including the '\n' character.


struct Position
{
    i32 row;
    i32 col;
};


void Day11()
{
    fprintf(stdout, "Day 11\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading file: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    u32 galaxy_count = 0;
    Position galaxy_map[kMaxGalaxies] = {};
    
    bool row_has_galaxy[kMaxRows] = {};
    bool col_has_galaxy[kMaxCols] = {};
    
    i32 rows = 0;
    i32 cols = 0;
    while (!AtEndOfFile(&parser)) {
        cols = 0;
        while (!AtEndOfLine(&parser)) {
            char current_char = parser.data[parser.offset];
            if (current_char == '#') {
                galaxy_map[galaxy_count] = { rows, cols };
                galaxy_count++;
                col_has_galaxy[cols] = true;
                row_has_galaxy[rows] = true;
            }
            cols++;
            Advance(&parser);
        }
        rows++;
        Advance(&parser);
    }
    
    fprintf(stdout, "Galaxy count: %u (r: %u, c: %u)\n", galaxy_count, rows, cols);
    
    i32 empty_row_count = 0;
    fprintf(stdout, "Empty rows:  ");
    for (int i = 0; i < rows; ++i) {
        if (!row_has_galaxy[i]) {
            empty_row_count++;
            fprintf(stdout, "%d ", i);
        }
    }
    fprintf(stdout, " (%d)\n", empty_row_count);

    i32 empty_col_count = 0;
    fprintf(stdout, "Empty cols:  ");
    for (int i = 0; i < cols; ++i) {
        if (!col_has_galaxy[i]) {
            empty_col_count++;
            fprintf(stdout, "%d ", i);
        }
    }
    fprintf(stdout, " (%d)\n", empty_col_count);
    
    // Expand the positions of the galaxies.
    // Create a list of adjustments for each column.
    i32 col_adjustment[kMaxCols] = {};
    i32 current_adjustment = 0;
    for (int i = 0; i < cols; ++i) {
        if (!col_has_galaxy[i]) {
            current_adjustment++;
        }
        col_adjustment[i] = current_adjustment;
    }

    // Create a list of adjustments for each row.
    i32 row_adjustment[kMaxRows] = {};
    current_adjustment = 0;
    for (int i = 0; i < rows; ++i) {
        if (!row_has_galaxy[i]) {
            current_adjustment++;
        }
        row_adjustment[i] = current_adjustment;
    }
    
    // Adjust the positions of each galaxy.
    for (int i = 0; i < galaxy_count; ++i) {
        Position old_pos = galaxy_map[i];
        Position new_pos = { old_pos.row + row_adjustment[old_pos.row],
                             old_pos.col + col_adjustment[old_pos.col] };
        galaxy_map[i] = new_pos;
        // fprintf(stdout, "(%u, %u) --> (%u, %u)\n", old_pos.row, old_pos.col, new_pos.row, new_pos.col);
    }
    
    // Compute the distances between each pair of galaxies.
    u32 distance_sum = 0;
    u32 count = 0;
    for (int i = 0; i < galaxy_count; ++i) {
        Position g1 = galaxy_map[i];
        for (int j = i; j < galaxy_count; ++j) {
            Position g2 = galaxy_map[j];
            u32 distance = abs(g2.row - g1.row) + abs(g2.col - g1.col);
            // fprintf(stdout, "%d: (%u, %u) - (%u, %u) = %u\n", count, g2.row, g2.col, g1.row, g1.col, distance);
            distance_sum += distance;
            count++;
        }
    }

    fprintf(stdout, "Day 11: distance sum: %u\n", distance_sum);

    CloseFile(&input_file);
}

