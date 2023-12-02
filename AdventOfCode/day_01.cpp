// day_01.cpp
// AdventOfCode
//
// Solution for: https://adventofcode.com/2023/day/1
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include <stdio.h>


struct KeyWord
{
    char* start;
    int length;
};


#define KEYWORD(x) { (char*) x, sizeof(x) - 1 }


const KeyWord kNumbers[] = {
    KEYWORD("zero"),
    KEYWORD("one"),
    KEYWORD("two"),
    KEYWORD("three"),
    KEYWORD("four"),
    KEYWORD("five"),
    KEYWORD("six"),
    KEYWORD("seven"),
    KEYWORD("eight"),
    KEYWORD("nine")
};


const size_t kLineBufferSize = 1024;

const char* input_file = "day_01.input";  // 55017 (part 1), 53539 (part 2)
// const char* input_file = "day_01.test_input1";  // 142 (part 1)
// const char* input_file = "day_01.test_input2";  // 281 (part 2)


bool MatchNumberKeyword(const char* line, int start_index, KeyWord keyword)
{
    bool match = true;
    
    for (int index = 0; index < keyword.length; ++index, ++start_index) {
        if (line[start_index] != keyword.start[index]) {
            match = false;
            break;
        }
    }
    
    return match;
}


int GetCalibrationValueFromLine(const char* line, ssize_t line_size, int current_line_count)
{
    int first_digit = -1;
    int last_digit = -1;
    
    int index = 0;
    bool end_of_line = false;
    
    while (index < line_size && !end_of_line) {
        int digit = -1;
        int index_advance = 1;
        const char current_char = line[index];
        switch (current_char) {
            case '\n': {
                end_of_line = true;
                index_advance = 0;
                break;
            }
                
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
                digit = current_char - '0';
                break;
            }

            case 'e':
            case 'f':
            case 'n':
            case 'o':
            case 's':
            case 't':
            case 'z': {
                for (int nindex = 0; nindex < 10; ++nindex) {
                    if (MatchNumberKeyword(line, index, kNumbers[nindex])) {
                        digit = nindex;
                        break;
                    }
                }
                break;
            }

            default: {
                break;
            }
        }
        
        if (digit >= 0 && digit < 10) {
            if (first_digit < 0) {
                first_digit = digit;
            }
            last_digit = digit;
        }
        
        index += index_advance;
    }

    int calibration_value = first_digit * 10 + last_digit;
    
    fprintf(stdout, "%d: %.*s (%d bytes) -> (%d, %d) = %d\n",
            current_line_count, (int) index, line, (int) index,
            first_digit, last_digit, calibration_value);

    return calibration_value;
}


void Day01()
{
    FILE* f = fopen(input_file, "r");
    if (!f) {
        fprintf(stderr, "Failed to open: %s\n", input_file);
    }

    size_t line_buffer_size = kLineBufferSize;
    char* line_buffer = new char[line_buffer_size];

    int calibration_value = 0;
    
    int line_count = 0;
    ssize_t bytes_read = 0;
    while (bytes_read >= 0) {
        bytes_read = getline(&line_buffer, &line_buffer_size, f);
        if (bytes_read >= 0) {
            line_count++;
            calibration_value += GetCalibrationValueFromLine(line_buffer, line_buffer_size, line_count);
        }
    }
    
    fprintf(stdout, "Calibration value: %d (%d lines from %s)\n",
            calibration_value, line_count, input_file);
    
    delete [] line_buffer;
    
    fclose(f);
}
