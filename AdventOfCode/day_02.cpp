// day_02.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <stdio.h>

static const char* input_file = "day_02.input";  // Sum: 2006, Fewest: 84911
// static const char* input_file = "day_02.test_input1";  // Sum: 8, Fewest: 2286

const int kLineSize = 1024;
const int kMaxGamesList = 16;


struct Game
{
    int red;
    int green;
    int blue;
};


struct GameList
{
    int id;
    int count;
    Game games[kMaxGamesList];
};


static KeyWord kRed = KEYWORD("red");
static KeyWord kGreen = KEYWORD("green");
static KeyWord kBlue = KEYWORD("blue");

Game test_case = { 12, 13, 14 };


GameList ParseLine(const char* line, size_t line_length)
{
    GameList results = {};
    
    // Parse the game number.
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
    
    results.id = game_id;
    
    // Parse each game.
    bool end_of_line = false;
    while (!end_of_line) {       
        Game game = {};
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
                case '\n': {
                    game_complete = true;
                    end_of_line = true;
                    advance = 0;
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
        
        results.games[results.count] = game;
        results.count++;
    }
    
    return results;
}


bool GamesListIsPossible(GameList* game_list, Game test_case)
{
    bool result = true;
    
    for (int index = 0; index < game_list->count; ++index) {
        Game current = game_list->games[index];
        if (current.red > test_case.red ||
            current.green > test_case.green ||
            current.blue > test_case.blue) {
            result = false;
            break;
        }
    }
    
    return result;
}


int FindMinimumValues(GameList* game_list)
{
    int result = 0;
    
    Game mins = {};
    
    for (int index = 0; index < game_list->count; ++index) {
        Game current = game_list->games[index];
        if (current.red > mins.red) {
            mins.red = current.red;
        }
        if (current.green > mins.green) {
            mins.green = current.green;
        }
        if (current.blue > mins.blue) {
            mins.blue = current.blue;
        }
    }
    
    result = mins.red * mins.green * mins.blue;
    
    return result;
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
    
    int game_id_sum = 0;
    int fewest_sum = 0;
    
    ssize_t bytes_read = 0;
    while (bytes_read >= 0) {
        bytes_read = getline(&line, &line_size, f);
        if (bytes_read >= 0) {
            GameList results = ParseLine(line, bytes_read);
            // for (int i = 0; i < results.count; ++i) {
            //    Game game = results.games[i];
            //    fprintf(stdout, "id: %d, r: %d, g: %d, b: %d\n", results.id, game.red, game.green, game.blue);
            // }
            if (GamesListIsPossible(&results, test_case)) {
                game_id_sum += results.id;
            }
            fewest_sum += FindMinimumValues(&results);
        }
    }
    
    fprintf(stdout, "Sum: %d, fewest: %d\n", game_id_sum, fewest_sum);
    
    delete [] line;
    fclose(f);
}


