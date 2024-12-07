// advent_of_code.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <errno.h>
#include <sys/stat.h>

#include <dispatch/dispatch.h>
#include <mach/mach_time.h>


static f64 g_scale = 1.0f;
static mach_timebase_info_data_t g_time_base;


u64 TimeNow()
{
    u64 result = mach_absolute_time();
    return result;
}


f64 MillisecondsSince(u64 start_time)
{
    f64 result = mach_absolute_time();
    
    static dispatch_once_t time_init_token;

    dispatch_once(&time_init_token, ^{
        mach_timebase_info(&g_time_base);
        if (g_time_base.denom > 0) {
            g_scale = ((f64) (g_time_base.numer / g_time_base.denom)) / NSEC_PER_MSEC;
        }
    });
    
    result = (result - start_time) * g_scale;

    return result;
}



bool operator<(const String& lhs, const String& rhs)
{
    size_t smaller = (lhs.size < rhs.size) ? lhs.size : rhs.size;

    for (int i = 0; i < smaller; ++i) {
        if (lhs.start[i] < rhs.start[i]) {
            return true;
        } else if (lhs.start[i] > rhs.start[i]) {
            return  false;
        }
    }
    
    return (lhs.size < rhs.size);
}


bool operator==(const String& lhs, const String& rhs)
{
    bool result = true;
    
    if (lhs.size == rhs.size) {
        if (lhs.start != rhs.start) {
            for (int i = 0; i < lhs.size; ++i) {
                if (lhs.start[i] != rhs.start[i]) {
                    result = false;
                    break;
                }
            }
        }
    } else {
        result = false;
    }
    
    return result;
}


// From http://www.azillionmonkeys.com/qed/hash.html

#define get16bits(d) ((((u32)(((const u8 *)(d))[1])) << 8)\
                       +(u32)(((const u8 *)(d))[0]) )

u32 SuperFastHash(const char* data, int len)
{
    u32 hash = len, tmp;
    int rem;
    
    if (len <= 0 || data == NULL) {
        return 0;
    }
    
    rem = len & 3;
    len >>= 2;
    
    // Main loop
    for (; len > 0; len--) {
        hash += get16bits(data);
        tmp   = (get16bits(data+2) << 11) ^ hash;
        hash  = (hash << 16) ^ tmp;
        data += 2 * sizeof(uint16_t);
        hash += hash >> 11;
    }
    
    // Handle end cases
    switch (rem) {
        case 3:
            hash += get16bits(data);
            hash ^= hash << 16;
            hash ^= ((signed char) data[sizeof(uint16_t)]) << 18;
            hash += hash >> 11;
            break;
        case 2:
            hash += get16bits(data);
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        case 1:
            hash += (signed char) *data;
            hash ^= hash << 10;
            hash += hash >> 1;
    }
    
    // Force "avalanching" of final 127 bits
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    
    return hash;
}


size_t StringHash::operator()(const String& s) const
{
    size_t result = SuperFastHash(s.start, (i32) s.size);
    return result;
}


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


i64 ParseNumberInternal(ParseState* parser, bool allow_sign)
{
    u64 number = 0;
    bool number_found = false;
    bool done = false;
    bool is_negative = false;
    while (!done && !AtEndOfFile(parser)) {
        int advance = 1;
        char current = parser->data[parser->offset];
        switch (current) {
            case '-': {
                if (allow_sign) {
                    is_negative = true;
                }
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
                u64 digit = current - '0';
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
                    if (is_negative) {
                        number *= -1;
                    }
                    done = true;
                    advance = 0;
                }
                if (is_negative) {
                    is_negative = false;
                }
            }
        }
        parser->offset += advance;
    }
    
    return number;
}


u64 ParseNumber(ParseState* parser)
{
    u64 number = (u64) ParseNumberInternal(parser, false);    
    return number;
}


i64 ParseSignedNumber(ParseState* parser)
{
    i64 result = ParseNumberInternal(parser, true);
    return result;
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


String ParseUntilSpace(ParseState* parser)
{
    String result = {};
    
    const char* start = parser->data + parser->offset;
    size_t size = 0;
    
    i32 advance = 1;
    bool done = false;
    while (!done && !AtEndOfFile(parser)) {
        char current_char = parser->data[parser->offset];
        if (current_char == ' ') {
            done = true;
            advance = 0;
            result = { start, size };
        }
        Advance(parser, advance);
        size++;
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


