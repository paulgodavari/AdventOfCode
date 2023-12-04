// day_04.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file = "day_04.test_input";  // Part 1: 13, part 2:
// static const char* input_file = "day_04.input";  // Part 1: 20829, part 2:


void Day04()
{
    fprintf(stdout, "Day04\n");
    
    File input = ReadFile(input_file);
    if (!input.handle) {
        fprintf(stderr, "Error reading file\n");
        return;
    }
    
    int total_points = 0;
    
    int index = 0;
    while (index < input.size) {
        int game = 0;
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
                    game = game * 10 + digit;
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

        fprintf(stdout, "Game %d: ", game);

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

        fprintf(stdout, " | ");

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
                
        total_points += game_points;
        fprintf(stdout, " | points: %d\n", game_points);
    }
    
    fprintf(stdout, "Total points: %d\n", total_points);
}


