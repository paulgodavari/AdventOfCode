// day_02.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <stdio.h>

// static const char* input_file = "day_02.input";
static const char* input_file = "day_02.test_input1";
// static const char* input_file = "day_02.test_input2";

const int kLineSize = 1024;


struct Game
{
    int id;
    int red;
    int green;
    int blue;
};


static KeyWord kRed = KEYWORD("red");
static KeyWord kGreen = KEYWORD("green");
static KeyWord kBlue = KEYWORD("blue");


void ParseLine(const char* line, size_t line_length)
{
    // Parse the game number.
    Game game = {};

    int game_id = 0;
    bool found_game_id = false;
    const char* current_char = line;
    while (current_char && !found_game_id) {
        switch (*current_char) {
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
                int digit = *current_char - '0';
                game_id = game_id * 10 + digit;
                break;
            }
            case ':': {
                found_game_id = true;
                break;
            }
                
        }
        current_char++;
    }
    
    game.id = game_id;
    
    // Parse each game.
    
    bool game_complete = false;
    int value = 0;
    while (current_char && !game_complete) {
        int advance = 1;
        switch (*current_char) {
            case ';': {
                game_complete = true;
                break;
            }
            case ',': {
                value = 0;
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
                int digit = *current_char - '0';
                value = value * 10 + digit;
                break;
            }
            case 'r': {
                if (MatchNumberKeyword(current_char, 0, kRed)) {
                    advance = kRed.length;
                    game.red = value;
                }
                break;
            }
            case 'g': {
                if (MatchNumberKeyword(current_char, 0, kGreen)) {
                    advance = kGreen.length;
                    game.green = value;
                }
                break;
            }
            case 'b': {
                if (MatchNumberKeyword(current_char, 0, kBlue)) {
                    advance = kBlue.length;
                    game.blue = value;
                }
                break;
            }
            default:
                break;
        }
        
        current_char += advance;
    }
    
    fprintf(stdout, "id: %d, r: %d, g: %d, b: %d\n", game.id, game.red, game.green, game.blue);
}


void Day02()
{
    FILE* f = fopen(input_file, "r");
    if (!f) {
        fprintf(stderr, "Error opening: %s\n", input_file);
        return;
    }
    
    char* line = new char[kLineSize];
    size_t line_size = kLineSize;
    
    ssize_t bytes_read = 0;
    while (bytes_read >= 0) {
        bytes_read = getline(&line, &line_size, f);
        if (bytes_read >= 0) {
            ParseLine(line, bytes_read);
        }
    }
        
    delete [] line;
    fclose(f);
}


