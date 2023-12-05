// day_05.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#define TESTING 1

#ifdef TESTING
    static const char* input_file = "day_05.test_input";
    static const int kMapSize = 100;
#else
    static const char* input_file = "day_05.input";
    static const int kMapSize = 1000000000;
#endif


void Day05()
{
    // Seed numbers
    File input_data = ReadFile(input_file);
    if (!input_data.handle) {
        fprintf(stderr, "Can't read input file\n");
        return -1;
    }
    
    const char* data = input_data.data;
    int index = 0;
    bool end_of_line = false;
    int number = 0;
    while (!end_of_line) {
        char current = data[index];
        switch (current) {
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
                int digit = current - '0';
                number = number * 10 + digit;
            }
            default: {
            }
        }
    }
}


