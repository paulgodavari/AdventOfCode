// day_07.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_07.test_input";
// static const char* input_file_name = "day_07.input";


static const u32 kMaxGames = 1024;


struct Game
{
    String hand;
    u32 bid;
};


void Day07()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    u32 game_count = 0;
    Game games[kMaxGames] = {};
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    while (!AtEndOfFile(&parser)) {
        String hand = ParseWord(&parser);
        Advance(&parser);  // Space
        u32 bid = ParseNumber(&parser);
        assert(AtEndOfLine(&parser));
        Advance(&parser);  // End of Line
        
        games[game_count] = { hand, bid };
        game_count++;
    }
    
    fprintf(stdout, "Game count: %u\n", game_count);
    for (int i = 0; i < game_count; ++i) {
        fprintf(stdout, "Game %u: %.*s -> %u\n", i, (int) games[i].hand.size, games[i].hand.start, games[i].bid);
    }
    
    CloseFile(&input_file);
}

