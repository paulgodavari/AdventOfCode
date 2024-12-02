// day_13.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


// static const char* input_file_name = "day_13.test_input";  // Part 1: 405, part 2: 400
static const char* input_file_name = "day_13.input";  // Part 1: 27502, part 2: 31947


struct Reflection
{
    u32 rows_above;
    u32 cols_left;
};


bool operator==(const Reflection& lhs, const Reflection& rhs)
{
    return (lhs.cols_left == rhs.cols_left) && (lhs.rows_above == rhs.rows_above);
}


bool Matches(const Reflection& lhs, const Reflection& rhs)
{
    bool result = false;
    if (rhs.cols_left > 0 && (rhs.cols_left == lhs.cols_left)) {
        result = true;
    } else if (rhs.rows_above > 0 && (rhs.rows_above == lhs.rows_above)) {
        result = true;
    }
    return result;
}


struct Pattern
{
    char* start;
    u32 rows;
    u32 cols;
    Reflection part1;
    Reflection part2;
};


bool ColumnsAreIdentical(Pattern pattern, u32 column1, u32 column2)
{
    bool result = true;
    
    char* col1 = pattern.start + column1;
    char* col2 = pattern.start + column2;
    for (int row = 0; row < pattern.rows; ++row) {
        u32 row_offset = row * (pattern.cols + 1);
        char c1 = *(col1 + row_offset);
        char c2 = *(col2 + row_offset);
        if (c1 != c2) {
            result = false;
            break;
        }
    }
    
    return result;
}


bool RowsAreIdentical(Pattern pattern, u32 row1, u32 row2)
{
    bool result = true;
    
    char* row1_start = pattern.start + row1 * (pattern.cols + 1);
    char* row2_start = pattern.start + row2 * (pattern.cols + 1);;
    for (int col = 0; col < pattern.cols; ++col) {
        char c1 = *(row1_start + col);
        char c2 = *(row2_start + col);
        if (c1 != c2) {
            result = false;
            break;
        }
    }
    
    return result;
}


bool IsReflectionColumn(Pattern pattern, u32 column)
{
    bool result = true;
    
    i32 col_left = column - 1;
    i32 col_right = (column +1 ) + 1;
    while ((col_left >= 0) && (col_right < pattern.cols)) {
        result = ColumnsAreIdentical(pattern, col_left, col_right);
        if (!result) {
            break;
        }
        col_left--;
        col_right++;
    }
    
    return result;
}


bool IsReflectionRow(Pattern pattern, u32 row)
{
    bool result = true;
    
    i32 row_up = row - 1;
    i32 row_down = row + 2;
    while ((row_up >= 0) && (row_down < pattern.rows)) {
        result = RowsAreIdentical(pattern, row_up, row_down);
        if (!result) {
            break;
        }
        row_up--;
        row_down++;
    }
    
    return result;
}


Reflection FindReflection(Pattern pattern, bool part2 = false)
{
    Reflection result = {};
    
    // Find vertical reflection.
    for (int col = 0; col < (pattern.cols - 1); ++col) {
        bool found_horizontal = ColumnsAreIdentical(pattern, col, col+1);
        if (found_horizontal) {
            if (part2 && pattern.part1.cols_left == (col + 1)) {
                continue;
            }
            // fprintf(stdout, "Column match at: %u, %u\n", col, col+1);
            if (IsReflectionColumn(pattern, col)) {
                // fprintf(stdout, "Column reflection at: %u, %u\n", col, col+1);
                result.cols_left = col + 1;
                break;
            }
        }
    }
    
    // Find horizontal reflection.
    for (int row = 0; row < (pattern.rows - 1); ++row) {
        bool found_vertical = RowsAreIdentical(pattern, row, row+1);
        if (found_vertical) {
            if (part2 && pattern.part1.rows_above == (row + 1)) {
                continue;
            }
            // fprintf(stdout, "Row match at: %u, %u\n", row, row+1);
            if (IsReflectionRow(pattern, row)) {
                // fprintf(stdout, "Row reflection at: %u, %u\n", row, row+1);
                result.rows_above = row + 1;
                break;
            }
        }
    }
    
    return result;
}


void Day13()
{
    fprintf(stdout, "Day 13\n");
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    u32 part_1_sum = 0;
    u32 part_2_sum = 0;

    ParseState parser = { input_file.data, input_file.size, 0 };
    
    while (!AtEndOfFile(&parser)) {
        
        // Read each pattern.
        Pattern pattern = { parser.data + parser.offset, 0, 0 };
        bool reading_pattern = true;
        u32 col_count = 0;
        while (!AtEndOfLine(&parser) && reading_pattern) {
            col_count++;
            Advance(&parser);
            if (AtEndOfLine(&parser)) {
                if (pattern.cols == 0) {
                    pattern.cols = col_count;
                }
                col_count = 0;
                pattern.rows++;
                Advance(&parser);
                if (AtEndOfLine(&parser)) {
                    // Found a blank line indicating end of pattern.
                    Advance(&parser);
                    reading_pattern = false;
                }
                if (AtEndOfFile(&parser)) {
                    // Last pattern found.
                    reading_pattern = false;
                }
            }
        }
        
        Reflection rp1 = FindReflection(pattern);
        part_1_sum += rp1.cols_left + rp1.rows_above * 100;
        pattern.part1 = rp1;
        
        // fprintf(stdout, "Part 1: found pattern sum: %u (r: %u, c: %u)\n",
        //         part_1_sum, rp1.rows_above, rp1.cols_left);

        // Part 2: toggle every point in each pattern to fix the smudge.
        Reflection current_r2 = {};
        bool found_sum = false;
        for (int row = 0; row < pattern.rows; ++row) {
            for (int col = 0; col < pattern.cols; ++col) {
                u32 offset = row * (pattern.cols + 1) + col;
                pattern.start[offset] = (pattern.start[offset] == '.') ? '#' : '.';  // Fix candidate smudge
                Reflection rp2 = FindReflection(pattern, true);
                pattern.start[offset] = (pattern.start[offset] == '.') ? '#' : '.';  // Put candidate back
                
                Reflection new_r = rp2;
                if (new_r == current_r2) {
                    continue;
                }
                current_r2 = new_r;
                
                u32 r2_sum = new_r.cols_left + new_r.rows_above * 100;
                if (r2_sum && !found_sum) {
                    found_sum = true;
                    part_2_sum += r2_sum;
                    // fprintf(stdout, "Part 2: found pattern sum: %u (r: %u, c: %u)\n",
                    //         part_2_sum, new_r.rows_above, new_r.cols_left);
                }
            }
        }
    }
    
    fprintf(stdout, "Part 1: %u\n", part_1_sum);
    fprintf(stdout, "Part 2: %u\n", part_2_sum);

    CloseFile(&input_file);
}


