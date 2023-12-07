// advent_of_code.h
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include <stdint.h>
#include <stdio.h>


typedef  int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef  uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;


struct KeyWord
{
    char* start;
    int length;
};
#define KEYWORD(x) { (char*) x, sizeof(x) - 1 }


bool MatchNumberKeyword(const char* line, int line_index, KeyWord keyword);


struct File
{
    FILE* handle;
    char* data;
    size_t size;
};


void CloseFile(File* file);
File ReadFile(const char* file_name);


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
    i32 offset;
};


// Consumes leading non-digit characters, converts digit characters into a number,
// returns the number. Parser will be pointing 1 character past the last digit.
u32 ParseNumber(ParseState* parse_state);

// Attempts to match the provided string. If there is a complete match, the parser
// is advanced 1 character past the match. The state is not updated if there is no
// match, or a partial match.
bool ConsumeString(ParseState* parser_state, String match);

// Returns true if the parse_state's current offset is a '\n' character.
bool AtEndOfLine(ParseState* parse_state);

// Moves the parse_state ahead by the specified number of bytes. It will not
// advance if 'by' takes it beyond 'size'.
void Advance(ParseState* parse_state, int by = 1);


void Day01();
void Day02();
void Day03();
void Day04();
void Day05();
void Day06();
void Day07();
