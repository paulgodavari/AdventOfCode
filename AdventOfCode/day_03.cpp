// day_03.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <errno.h>
#include <sys/stat.h>


// static const char* input_file = "day_03.test_input";  // Part 1 sum = 4361, Part 2 sum = 467835
static const char* input_file = "day_03.input";  // Part 1 sum = 544433, Part 2 sum = 76314915


struct File
{
    FILE* handle;
    char* data;
    size_t size;
};


struct Table
{
    const char* data;
    int rows;
    int cols;
};


struct Number
{
    int value;
    int row;
    int col_start;
    int col_end;
};


struct Star
{
    int row;
    int col;
    int num1;
    int num2;
};


// These were determined by counting through the input data. In real code,
// I'd need to use a dynamic array.
static const int kMaxStars = 512;
static const int kMaxNumbers = 1300;


struct StarTable
{
    int count;
    Star stars[kMaxStars];
};


struct NumberTable
{
    int count;
    Number numbers[kMaxNumbers];
};


struct Point
{
    int row;
    int col;
};


struct Box
{
    Point min;
    Point max;
};


Box ComputeBoundingBox(int row, int col_start, int col_end, int row_max, int col_max)
{
    Box result = {};

    result.min.row = row - 1;
    if (result.min.row < 0) {
        result.min.row = 0;
    }
    
    result.max.row = row + 1;
    if (result.max.row >= row_max) {
        result.max.row = row;
    }
    
    result.min.col = col_start - 1;
    if (result.min.col < 0) {
        result.min.col = 0;
    }
    
    result.max.col = col_end + 1;
    if (result.max.col >= col_max) {
        result.max.col = col_end;
    }

    return result;
}


void CloseFile(File* file)
{
    if (file) {
        if (file->handle) {
            fclose(file->handle);
        }
        if (file->data) {
            delete [] file->data;
        }
        *file = {};
    }
}


File ReadFile(const char* file_name)
{
    File result;
    struct stat file_info = {};
    if (!stat(file_name, &file_info)) {
        FILE* f = fopen(file_name, "r");
        if (f) {
            result.handle = f;
            result.size = file_info.st_size;
            result.data = new char[result.size];
            size_t bytes_read = fread(result.data, 1, result.size, result.handle);
            if (bytes_read != result.size ) {
                fprintf(stderr, "Error reading %s: read: %zu bytes, expecting: %zu bytes\n",
                        file_name, bytes_read, result.size);
                CloseFile(&result);
            }
        } else {
            fprintf(stderr, "Error opening %s (errno: %d)\n", file_name, errno);
        }
    } else {
        fprintf(stderr, "Error stat'ing %s (errno: %d)\n", file_name, errno);
    }
    
    return result;
}


bool IsSymbolAdjacent(Table t, int row_start, int col_start, int col_end)
{
    bool found_symbol = false;
    
    Box box = ComputeBoundingBox(row_start, col_start, col_end, t.rows, t.cols);
    
    for (int row = box.min.row; row <= box.max.row; ++row) {
        const char* data = t.data + row * t.cols;
        for (int col = box.min.col; col <= box.max.col; ++col) {
            char c = data[col];
            switch (c) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '\n':
                case '.': {
                    break;
                }
                default:
                    return true;
                    break;
            }
        }
    }
    
    return found_symbol;
}


bool IsAdjacent(Table* table, Star star, Number num)
{
    bool result = false;
    
    Box box = ComputeBoundingBox(num.row, num.col_start, num.col_end, table->rows, table->cols);
    if ((star.row >= box.min.row && star.row <= box.max.row) &&
        (star.col >= box.min.col && star.col <= box.max.col)) {
        result = true;
    }
    
    return result;
}


void Day03()
{
    File file_data = ReadFile(input_file);
    
    if (!file_data.handle) {
        return;
    }
    
    // column_count does not include the final column that contains '\n' characters.
    // To do calculations with file size or getting the position of the next row in memory,
    // you need to use 'column_count + 1'.
    int column_count = -1;
    
    for (int index = 0; index < file_data.size; ++index) {
        if (file_data.data[index] == '\n') {
            column_count = index;
            break;
        }
    }
    
    if (column_count < 0) {
        fprintf(stderr, "No newlines detected\n");
        CloseFile(&file_data);
        return;
    }
    
    int row_count = (int) file_data.size / (column_count + 1);
    
    Table table = { file_data.data, row_count, column_count + 1 };
    
    fprintf(stdout, "Rows: %d, columns: %d\n", row_count, column_count);
    
    int number_sum = 0;
    
    int number_count = 0;
    
    StarTable stars = {};
    NumberTable numbers = {};
    
    for (int row = 0; row < row_count; ++row) {
        const char* row_data = file_data.data + row * (column_count + 1);
        int num_start_col = -1;
        int num_end_col = -1;
        int num_value = 0;
        // Have to search for 1 past the column count (which is the column that
        // contains the '\n' characters) to terminate numbers that end at the
        // end of the row.
        for (int col = 0; col <= column_count; ++col) {
            char current_char = row_data[col];
            switch (current_char) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9': {
                    int digit = current_char - '0';
                    if (num_start_col < 0) {
                        num_start_col = col;
                        number_count++;
                    }
                    num_end_col = col;
                    num_value = num_value * 10 + digit;
                    break;
                }
                default: {
                    if (current_char == '*') {
                        stars.stars[stars.count] = { row, col };
                        stars.count++;
                    }
                    if (num_start_col >= 0 && num_end_col >= 0) {
                        fprintf(stdout, "Found %d at row: %d, col: %d..%d",
                                num_value, row, num_start_col, num_end_col);
                        numbers.numbers[numbers.count] = { num_value, row, num_start_col, num_end_col };
                        numbers.count++;
                        if (IsSymbolAdjacent(table, row, num_start_col, num_end_col)) {
                            number_sum += num_value;
                            fprintf(stdout, " symbol adjacent");
                        } else {
                            // fprintf(stdout, " not adjacent");
                        }
                        fprintf(stdout, "\n");
                        num_start_col = -1;
                        num_end_col = -1;
                        num_value = 0;
                    }
                }
            }
        }
    }
    
    // Part 2
    int gear_ratio_sum = 0;
    
    for (int star_index = 0; star_index < stars.count; ++star_index) {
        Star star = stars.stars[star_index];
        int adjacent_count = 0;
        for (int num_index = 0; num_index < numbers.count; ++num_index) {
            Number num = numbers.numbers[num_index];
            if (IsAdjacent(&table, star, num)) {
                adjacent_count++;
                if (adjacent_count == 1) {
                    star.num1 = num.value;
                } else if (adjacent_count == 2) {
                    star.num2 = num.value;
                } else if (adjacent_count > 2) {
                    break;
                }
            }
        }
        if (adjacent_count == 2) {
            gear_ratio_sum += star.num1 * star.num2;
        }
    }
    
    fprintf(stdout, "Found %d stars, %d numbers\n", stars.count, numbers.count);
    fprintf(stdout, "Part 1 sum: %d\n", number_sum);
    fprintf(stdout, "Part 2 sum: %d\n", gear_ratio_sum);

    CloseFile(&file_data);
}

