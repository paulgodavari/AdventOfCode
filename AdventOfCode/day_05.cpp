// day_05.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


// static const char* input_file = "day_05.test_input";  // Part 1: 35, Part 2: 46
static const char* input_file = "day_05.input";  // Part 1: 51752125, Part 2: 12634632

static const int kRangeRows = 64;
static const int kMapCount = 7;


struct String
{
    const char* start;
    size_t size;
};


#define CONST_STRING(x) { (x), sizeof(x) - 1 }


struct Range
{
    u32 start;
    u32 end;
    i32 offset;
};


struct Map
{
    String name;
    u32 row_count;
    Range ranges[kRangeRows];
};


struct ParseState
{
    const char* data;
    size_t size;
    i32 offset;
};


// Consumes leading non-digit characters, converts digit characters into a number,
// returns the number. Parser will be pointing 1 character past the last digit.

u32 ParseNumber(ParseState* parse_state)
{
    u32 number = 0;
    bool number_found = false;
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
                u32 digit = current - '0';
                if (number_found) {
                    number = number * 10 + digit;
                } else {
                    number_found = true;
                    number = digit;
                }
                break;
            }
            default: {
                if (number_found) {
                    done = true;
                    advance = 0;
                }
            }
        }
        parse_state->offset += advance;
    }
    
    return number;
}


// Attempts to match the provided string. If there is a complete match, the parser
// is advanced 1 character past the match. The state is not updated if there is no
// match, or a partial match.

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
    u32 seeds[64] = {};
    
    if (!ConsumeString(&parse_state, CONST_STRING("seeds: "))) {
        fprintf(stderr, "Bad format\n");
        return;
    }
    
    while (!AtEndOfLine(&parse_state)) {
        u32 number = ParseNumber(&parse_state);
        seeds[seed_count] = number;
        seed_count++;
    }
    Advance(&parse_state, 2);  // End of seed number line + following blank line

//    fprintf(stdout, "Found %d seeds\n", seed_count);
//    for (int i = 0; i < seed_count; ++i) {
//        fprintf(stdout, "%u ", seeds[i]);
//    }
//    fprintf(stdout, "\n");

    Map maps[kMapCount] = {
        { CONST_STRING("seed-to-soil map:") },
        { CONST_STRING("soil-to-fertilizer map:") },
        { CONST_STRING("fertilizer-to-water map:") },
        { CONST_STRING("water-to-light map:") },
        { CONST_STRING("light-to-temperature map:") },
        { CONST_STRING("temperature-to-humidity map:") },
        { CONST_STRING("humidity-to-location map:") }
    };
    
    // Parse the ranges
    for (int i = 0; i < kMapCount; ++i) {
        if (!ConsumeString(&parse_state, maps[i].name)) {
            fprintf(stderr, "Bad name\n");
            return;
        }
        Advance(&parse_state);
        while (!AtEndOfLine(&parse_state)) {
            u32 dest_index = ParseNumber(&parse_state);
            u32 src_index = ParseNumber(&parse_state);
            u32 range = ParseNumber(&parse_state);
            Range r = { src_index, src_index + range - 1, (i32)(dest_index - src_index) };
            maps[i].ranges[maps[i].row_count] = r;
            maps[i].row_count++;
            Advance(&parse_state);

            // fprintf(stdout, "d: %d, i: %d, r: %d\n", dest_index, src_index, range);
            // fprintf(stdout, "start: %u, end: %u, offset: %d\n", r.start, r.end, r.offset);
        }
        // fprintf(stdout, "%.*s %u rows\n", (int) maps[i].name.size, maps[i].name.start, maps[i].row_count);
        Advance(&parse_state);  // Blank line
    }
    
    // TODO: Parallelize this, since part 2 takes a lot of time.
    
    // Look up the seed to location index
    u32 min_location = 0xffffffff;
    for (int i = 0; i < seed_count; ++ i) {
        u32 index = seeds[i];
        for (int map_index = 0; map_index < kMapCount; ++map_index) {
            Map map = maps[map_index];
            for (int row = 0; row < map.row_count; ++row) {
                Range r = map.ranges[row];
                if (index >= r.start && index <= r.end) {
                    index += r.offset;
                    break;
                }
            }
        }
        // fprintf(stdout, "Seed: %u, location: %u\n", seeds[i], index);
        if (min_location > index) {
            min_location = index;
        }
    }
    
    fprintf(stdout, "Part 1 min location: %u\n", min_location);
    
    min_location = 0xffffffff;
    int seed_index = 0;
    while (seed_index < seed_count) {
        u32 seed_start = seeds[seed_index];
        u32 seed_end = seed_start + seeds[seed_index + 1] - 1;
        fprintf(stdout, "Part 2 start: %u, end: %u\n", seed_start, seed_end);
        for (u32 seed = seed_start; seed <= seed_end; ++seed) {
            u32 index = seed;
            for (int map_index = 0; map_index < kMapCount; ++map_index) {
                Map map = maps[map_index];
                for (int row = 0; row < map.row_count; ++row) {
                    Range r = map.ranges[row];
                    if (index >= r.start && index <= r.end) {
                        index += r.offset;
                        break;
                    }
                }
            }
            // fprintf(stdout, "Seed: %u, location: %u\n", seed, index);
            if (min_location > index) {
                min_location = index;
            }
        }
        seed_index += 2;
    }

    fprintf(stdout, "Part 2 min location: %u\n", min_location);

    CloseFile(&input_data);
}


