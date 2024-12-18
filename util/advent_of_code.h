// advent_of_code.h
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include <assert.h>
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

typedef float  f32;
typedef double f64;


u64 TimeNow();
f64 MillisecondsSince(u64 start_time);


struct String
{
    const char* start;
    size_t size;
};
#define CONST_STRING(x) (String) { (x), sizeof(x) - 1 }

bool operator<(const String& lhs, const String& rhs);
bool operator==(const String& lhs, const String& rhs);

struct StringHash
{
    size_t operator()(const String& s) const;
};

bool MatchNumberKeyword(const char* line, int line_index, String keyword);


struct File
{
    FILE* handle;
    char* data;
    size_t size;
};


void CloseFile(File* file);
File ReadFile(const char* file_name);


struct ParseState
{
    char* data;
    size_t size;
    i32 offset;
};


// Consumes leading non-digit characters, converts digit characters into a number,
// returns the number. Parser will be pointing 1 character past the last digit.
u64 ParseNumber(ParseState* parser);
i64 ParseSignedNumber(ParseState* parser);

// Consumes leading non-alpha-digit characters. Parser will be pointing 1 character
// past the last alpha-digit.
String ParseWord(ParseState* parser);

// Return a string from the current position up to (but not including) the first
// space character. The parser will be left pointing the space character.
String ParseUntilSpace(ParseState* parser);

// Attempts to match the provided string. If there is a complete match, the parser
// is advanced 1 character past the match. The state is not updated if there is no
// match, or a partial match.
bool ConsumeString(ParseState* parser, String match);

// Returns true if the parser's current offset is a '\n' character.
bool AtEndOfLine(ParseState* parser);

// Returns true if the parser's current offset equal to the data size
// (1 character beyond the last character).
bool AtEndOfFile(ParseState* parser);

// Moves the parser ahead by the specified number of bytes. It will not
// advance if 'by' takes it beyond 'size'.
void Advance(ParseState* parser, i32 by = 1);


