// advent_of_code.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <errno.h>
#include <sys/stat.h>


bool MatchNumberKeyword(const char* line, int line_index, String keyword)
{
    bool match = true;
    
    for (int index = 0; index < keyword.size; ++index, ++line_index) {
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


u32 ParseNumber(ParseState* parser)
{
    u32 number = 0;
    bool number_found = false;
    bool done = false;
    while (!done && !AtEndOfFile(parser)) {
        int advance = 1;
        char current = parser->data[parser->offset];
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
        parser->offset += advance;
    }
    
    return number;
}


String ParseWord(ParseState* parser)
{
    String result = {};
    
    bool found_word_start = false;
    bool done = false;
    
    while (!done && !AtEndOfFile(parser)) {
        int advance_by = 1;
        char current_char = parser->data[parser->offset];
        bool is_digit = current_char >= '0' && current_char <= '9';
        bool is_char = (current_char >= 'A' && current_char <= 'Z') || (current_char >= 'a' && current_char <= 'z');
        if (is_char || is_digit) {
            if (!found_word_start) {
                found_word_start = true;
                result.start = parser->data + parser->offset;
            }
            result.size++;
        } else {
            if (found_word_start) {
                done = true;
                advance_by = 0;
            }
        }
        Advance(parser, advance_by);
    }
    
    return result;
}


bool ConsumeString(ParseState* parser, String match)
{
    bool result = false;
    
    if ((parser->offset + match.size) < parser->size) {
        for (int index = 0; index < match.size; ++index) {
            int parser_index = parser->offset + index;
            if (parser->data[parser_index] != match.start[index]) {
                return result;
            }
        }
        result = true;
        parser->offset += match.size;
    }
    
    return result;
}


bool AtEndOfLine(ParseState* parser)
{
    return parser->data[parser->offset] == '\n';
}


bool AtEndOfFile(ParseState* parser)
{
    return parser->offset == parser->size;
}


void Advance(ParseState* parser, i32 by)
{
    parser->offset += by;
    if (parser->offset > parser->size) {
        parser->offset = (i32) parser->size;
    }
}


