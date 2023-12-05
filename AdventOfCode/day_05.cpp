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


struct String
{
    const char* start;
    size_t size;
};


#define CONST_STRING(x) { (x), sizeof(x) - 1 }


struct ParseState
{
    const char* data;
    size_t size;
    int offset;
};


int ParseNumber(ParseState* parse_state)
{
    int number = -1;
    
    bool done = false;
    while (!done && (parse_state->offset < parse_state->size)) {
        int advance = 1;
        char current = parse_state->data[parse_state->offset];
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
                if (number < 0) {
                    number = digit;
                } else {
                    number = number * 10 + digit;
                }
                break;
            }
            default: {
                if (number >= 0) {
                    done = true;
                    advance = 0;
                }
            }
        }
        parse_state->offset += advance;
    }
    
    return number;
}


bool ConsumeString(ParseState* parser_state, String match)
{
    bool result = false;
    
    if ((parser_state->offset + match.size) < parser_state->size) {
        for (int index = 0; index < match.size; ++index) {
            int parser_index = parser_state->offset + index;
            if (parser_state->data[parser_index] != match.start[index]) {
                return result;
            }
        }
        result = true;
        parser_state->offset += match.size;
    }
    
    return result;
}


bool AtEndOfLine(ParseState* parse_state)
{
    return parse_state->data[parse_state->offset] == '\n';
}


void Advance(ParseState* parse_state, int by = 1)
{
    if (parse_state->offset < (parse_state->size - 1)) {
        parse_state->offset += by;
    }
}


void Day05()
{
    File input_data = ReadFile(input_file);
    if (!input_data.handle) {
        fprintf(stderr, "Can't read input file\n");
        return;
    }
    
    ParseState parse_state = { input_data.data, input_data.size, 0 };
    
    // Parse the seed numbers
    int seed_count = 0;
    int seeds[64] = {};
    
    if (!ConsumeString(&parse_state, CONST_STRING("seeds: "))) {
        fprintf(stderr, "Bad format\n");
        return;
    }
    
    while (!AtEndOfLine(&parse_state)) {
        int number = ParseNumber(&parse_state);
        seeds[seed_count] = number;
        seed_count++;
    }
    Advance(&parse_state, 2);  // End of seed number line + following blank line

    fprintf(stdout, "Found %d seeds\n", seed_count);
    for (int i = 0; i < seed_count; ++i) {
        fprintf(stdout, "%d ", seeds[i]);
    }
    fprintf(stdout, "\n");

    if (!ConsumeString(&parse_state, CONST_STRING("seed-to-soil map:\n"))) {
        fprintf(stderr, "Bad format\n");
        return;
    }
    while (!AtEndOfLine(&parse_state)) {
        int dest_index = ParseNumber(&parse_state);
        int src_index = ParseNumber(&parse_state);
        int range = ParseNumber(&parse_state);
        Advance(&parse_state);
        fprintf(stdout, "d: %d, i: %d, r: %d\n", dest_index, src_index, range);
    }
    Advance(&parse_state);  // Blank line
    
    if (!ConsumeString(&parse_state, CONST_STRING("soil-to-fertilizer map:\n"))) {
        fprintf(stderr, "Bad format\n");
        return;
    }
    while (!AtEndOfLine(&parse_state)) {
        int dest_index = ParseNumber(&parse_state);
        int src_index = ParseNumber(&parse_state);
        int range = ParseNumber(&parse_state);
        Advance(&parse_state);
        fprintf(stdout, "d: %d, i: %d, r: %d\n", dest_index, src_index, range);
    }
    Advance(&parse_state);  // Blank line

    if (!ConsumeString(&parse_state, CONST_STRING("fertilizer-to-water map:\n"))) {
        fprintf(stderr, "Bad format\n");
        return;
    }
    while (!AtEndOfLine(&parse_state)) {
        int dest_index = ParseNumber(&parse_state);
        int src_index = ParseNumber(&parse_state);
        int range = ParseNumber(&parse_state);
        Advance(&parse_state);
        fprintf(stdout, "d: %d, i: %d, r: %d\n", dest_index, src_index, range);
    }
    Advance(&parse_state);  // Blank line

    if (!ConsumeString(&parse_state, CONST_STRING("water-to-light map:\n"))) {
        fprintf(stderr, "Bad format\n");
        return;
    }
    while (!AtEndOfLine(&parse_state)) {
        int dest_index = ParseNumber(&parse_state);
        int src_index = ParseNumber(&parse_state);
        int range = ParseNumber(&parse_state);
        Advance(&parse_state);
        fprintf(stdout, "d: %d, i: %d, r: %d\n", dest_index, src_index, range);
    }
    Advance(&parse_state);  // Blank line

    if (!ConsumeString(&parse_state, CONST_STRING("light-to-temperature map:\n"))) {
        fprintf(stderr, "Bad format\n");
        return;
    }
    while (!AtEndOfLine(&parse_state)) {
        int dest_index = ParseNumber(&parse_state);
        int src_index = ParseNumber(&parse_state);
        int range = ParseNumber(&parse_state);
        Advance(&parse_state);
        fprintf(stdout, "d: %d, i: %d, r: %d\n", dest_index, src_index, range);
    }
    Advance(&parse_state);  // Blank line

    if (!ConsumeString(&parse_state, CONST_STRING("temperature-to-humidity map:\n"))) {
        fprintf(stderr, "Bad format\n");
        return;
    }
    while (!AtEndOfLine(&parse_state)) {
        int dest_index = ParseNumber(&parse_state);
        int src_index = ParseNumber(&parse_state);
        int range = ParseNumber(&parse_state);
        Advance(&parse_state);
        fprintf(stdout, "d: %d, i: %d, r: %d\n", dest_index, src_index, range);
    }
    Advance(&parse_state);  // Blank line

    if (!ConsumeString(&parse_state, CONST_STRING("humidity-to-location map:\n"))) {
        fprintf(stderr, "Bad format\n");
        return;
    }
    while (!AtEndOfLine(&parse_state)) {
        int dest_index = ParseNumber(&parse_state);
        int src_index = ParseNumber(&parse_state);
        int range = ParseNumber(&parse_state);
        Advance(&parse_state);
        fprintf(stdout, "d: %d, i: %d, r: %d\n", dest_index, src_index, range);
    }
    Advance(&parse_state);  // Blank line

}


