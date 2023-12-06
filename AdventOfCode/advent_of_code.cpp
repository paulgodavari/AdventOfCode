// advent_of_code.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <errno.h>
#include <sys/stat.h>


bool MatchNumberKeyword(const char* line, int line_index, KeyWord keyword)
{
    bool match = true;
    
    for (int index = 0; index < keyword.length; ++index, ++line_index) {
        if (line[line_index] != keyword.start[index]) {
            match = false;
            break;
        }
    }
    
    return match;
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


void Advance(ParseState* parse_state, int by)
{
    if (parse_state->offset < (parse_state->size - 1)) {
        parse_state->offset += by;
    }
}


