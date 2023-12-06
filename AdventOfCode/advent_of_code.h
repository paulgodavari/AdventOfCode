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


void Day01();
void Day02();
void Day03();
void Day04();
void Day05();
void Day06();
