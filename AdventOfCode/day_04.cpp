// day_04.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


// static const char* input_file = "day_04.test_input";  // Part 1: 13, part 2: 30
static const char* input_file = "day_04.input";  // Part 1: 20829, part 2: 12648035


static const int kMaxGames = 256;


struct Game
{
    int number;
    int wins;
    int copies;
};


struct Games
{
    int count;
    Game games[kMaxGames];
};


void Day04()
{
    File input = ReadFile(input_file);
    if (!input.handle) {
        fprintf(stderr, "Error reading file\n");
        return;
    }
    
    int total_points = 0;
    Games games = {};
    
    int index = 0;
    while (index < input.size) {
        int game_id = 0;
        int game_points = 0;
        int winning[100] = {};
        int numbers[100] = {};
        
        bool done_parsing_game = false;
        while (!done_parsing_game) {
            char current_char = input.data[index];
            // Find the current game.
            switch (current_char) {
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
                    int digit = current_char - '0';
                    game_id = game_id * 10 + digit;
                    break;
                }
                case ':': {
                    done_parsing_game = true;
                    break;
                }
                default:
                    break;
            }
            index++;
        }

        fprintf(stdout, "Game %d: ", game_id);

        bool done_parsing_win = false;
        int num = 0;
        while (!done_parsing_win) {
            char current_char = input.data[index];
            switch (current_char) {
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
                    int digit = current_char - '0';
                    num = num * 10 + digit;
                    break;
                }
                case '|': {
                    done_parsing_win = true;
                    break;
                }
                case ' ': {
                    if (num > 0) {
                        fprintf(stdout, " %2d", num);
                        winning[num] = 1;
                        num = 0;
                    }
                    break;
                }
                default:
                    break;
            }
            index++;
        }

        fprintf(stdout, " |");

        bool done_parsing_num = false;
        num = 0;
        while (!done_parsing_num) {
            char current_char = input.data[index];
            switch (current_char) {
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
                    int digit = current_char - '0';
                    num = num * 10 + digit;
                    break;
                }
                case '\n': {
                    done_parsing_num = true;
                    // Fall through to next case
                }
                case ' ': {
                    if (num > 0) {
                        fprintf(stdout, " %2d", num);
                        numbers[num] = 1;
                        num = 0;
                    }
                    break;
                }
                default:
                    break;
            }
            index++;
        }
        
        int wins = 0;
        for (int i = 0; i < 100; ++i) {
            if (numbers[i] && winning[i]) {
                wins++;
            }
        }
        if (wins) {
            game_points = 1;
            for (int i = 1; i < wins; i++) {
                game_points *= 2;
            }
        }
        
        Game* game = &games.games[games.count];
        game->number = game_id;
        game->wins = wins;
        game->copies = 1;
        
        games.count++;
                
        total_points += game_points;
        fprintf(stdout, " | points: %d, wins: %d, copies: %d\n", game_points, game->wins, game->copies);
    }
    
    fprintf(stdout, "Part 1 points: %d\n", total_points);
    
    for (int i = 0; i < games.count; ++i) {
        Game* game = &games.games[i];
        if (game->wins > 0) {
            for (int copy = 0; copy < game->copies; ++copy) {
                int index_base = i+1;
                for (int w = index_base; w < index_base + game->wins; ++w) {
                    games.games[w].copies++;
                }
            }
        }
    }
    
    int copies = 0;
    for (int i = 0; i < games.count; ++i) {
        copies += games.games[i].copies;
    }

    fprintf(stdout, "Part 2 copies: %d\n", copies);
}


