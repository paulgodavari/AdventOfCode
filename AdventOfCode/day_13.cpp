// day_13.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_13.test_input";  // Part 1: 405, part 2: 
// static const char* input_file_name = "day_13.input";  // Part 1: 27502, part 2: 


struct Pattern
{
    char* start;
    u32 rows;
    u32 cols;
};


struct Reflection
{
    u32 rows_above;
    u32 cols_left;
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


Reflection FindReflection(Pattern pattern)
{
    Reflection result = {};
    
    // Find vertical reflection.
    for (int col = 0; col < (pattern.cols - 1); ++col) {
        bool found_horizontal = ColumnsAreIdentical(pattern, col, col+1);
        if (found_horizontal) {
            // fprintf(stdout, "Column match at: %u, %u\n", col, col+1);
            if (IsReflectionColumn(pattern, col)) {
                fprintf(stdout, "Column reflection at: %u, %u\n", col, col+1);
                result.cols_left = col + 1;
                break;
            }
        }
    }
    
    // Find horizontal reflection.
    for (int row = 0; row < (pattern.rows - 1); ++row) {
        bool found_vertical = RowsAreIdentical(pattern, row, row+1);
        if (found_vertical) {
            // fprintf(stdout, "Row match at: %u, %u\n", row, row+1);
            if (IsReflectionRow(pattern, row)) {
                fprintf(stdout, "Row reflection at: %u, %u\n", row, row+1);
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
        
        Reflection reflection_point = FindReflection(pattern);
        part_1_sum += reflection_point.cols_left + reflection_point.rows_above * 100;
        
        fprintf(stdout, "Found pattern sum: %u (r: %u, c: %u)\n",
                part_1_sum, pattern.rows, pattern.cols);
    }
    
    fprintf(stdout, "Part 1: %u\n", part_1_sum);
    
    CloseFile(&input_file);
}


