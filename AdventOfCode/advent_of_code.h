// advent_of_code.h
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


struct KeyWord
{
    char* start;
    int length;
};


#define KEYWORD(x) { (char*) x, sizeof(x) - 1 }


bool MatchNumberKeyword(const char* line, int line_index, KeyWord keyword);


void Day01();
void Day02();


