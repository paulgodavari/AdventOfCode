// advent_of_code.h
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include <stdio.h>


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
