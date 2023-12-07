// day_07.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <stdlib.h>


// static const char* input_file_name = "day_07.test_input";  // Part 1 winnings: 6440, Part 2: 5905
static const char* input_file_name = "day_07.input";  // Part 1 winnings: 241344943, Part 2: 243101568


static const u32 kHandSize = 5;
static const u32 kMaxGames = 1024;


enum HandType
{
    kHandTypeHighCard = 0,
    kHandTypeOnePair,
    kHandTypeTwoPair,
    kHandTypeThree,
    kHandTypeFullHouse,
    kHandTypeFour,
    kHandTypeFive,
};


enum CardType
{
    kCardTypeJ = 0,
    kCardType2,
    kCardType3,
    kCardType4,
    kCardType5,
    kCardType6,
    kCardType7,
    kCardType8,
    kCardType9,
    kCardTypeT,
    kCardTypeQ,
    kCardTypeK,
    kCardTypeA,
    kCardTypeSize
};


struct Hand
{
    CardType cards[kHandSize];
};


struct Game
{
    String hand_str;
    Hand hand;
    u32 bid;
    HandType type;
};


HandType ClassifyHand(Hand hand)
{
    HandType result = kHandTypeHighCard;
    
    u32 cards[kCardTypeSize] = {};
    
    for (int i = 0; i < kHandSize; ++i) {
        CardType card_type = hand.cards[i];
        cards[card_type]++;
    }
    
    u32 js = cards[kCardTypeJ];
    u32 twos = 0;
    u32 threes = 0;
    u32 fours = 0;
    u32 fives = 0;
    for (int i = kCardType2; i < kCardTypeSize; ++i) {
        switch (cards[i]) {
            case 2:
                twos++;
                break;
            case 3:
                threes++;
                break;
            case 4:
                fours++;
                break;
            case 5:
                fives++;
                break;
            default:
                break;
        }
    }
    
    if (fives) {
        result = kHandTypeFive;
    } else if (fours) {
        result = (js > 0) ? kHandTypeFive : kHandTypeFour;
    } else if (threes) {
        if (js == 2) {
            result = kHandTypeFive;
        } else if (js == 1) {
            result = kHandTypeFour;
        } else {
            result = (twos > 0) ? kHandTypeFullHouse : kHandTypeThree;
        }
    } else if (twos) {
        if (js == 3) {
            result = kHandTypeFive;
        } else if (js == 2) {
            result = kHandTypeFour;
        } else if (js == 1) {
            result = (twos == 2) ? kHandTypeFullHouse : kHandTypeThree;
        } else {
            result = (twos == 2) ? kHandTypeTwoPair : kHandTypeOnePair;
        }
    } else if (js) {
        switch (js) {
            case 1:
                result = kHandTypeOnePair;
                break;
            case 2:
                result = kHandTypeThree;
                break;
            case 3:
                result = kHandTypeFour;
                break;
            case 4:
                result = kHandTypeFive;
                break;
            case 5:
                result = kHandTypeFive;
                break;
        }
    }
    
    return result;
}


Hand StringToHand(String input)
{
    assert(input.size == kHandSize);

    Hand result = {};

    for (int i = 0; i < kHandSize; ++i) {
        CardType card_type = {};
        switch (input.start[i]) {
            case 'A':
            case 'a':
                card_type = kCardTypeA;
                break;
            case 'K':
            case 'k':
                card_type = kCardTypeK;
                break;
            case 'Q':
            case 'q':
                card_type = kCardTypeQ;
                break;
            case 'J':
            case 'j':
                card_type = kCardTypeJ;
                break;
            case 'T':
            case 't':
                card_type = kCardTypeT;
                break;
            case '9':
                card_type = kCardType9;
                break;
            case '8':
                card_type = kCardType8;
                break;
            case '7':
                card_type = kCardType7;
                break;
            case '6':
                card_type = kCardType6;
                break;
            case '5':
                card_type = kCardType5;
                break;
            case '4':
                card_type = kCardType4;
                break;
            case '3':
                card_type = kCardType3;
                break;
            case '2':
                card_type = kCardType2;
                break;
            default: {
                assert(0);
                break;
            }
        }
        result.cards[i] = card_type;
    }
    
    return result;
}


int CardCompare(const void* lhs, const void* rhs)
{
    Game* g1 = (Game*) lhs;
    Game* g2 = (Game*) rhs;
    
    if (g1->type < g2->type) {
        return -1;
    } else if (g1->type > g2->type) {
        return 1;
    } else {
        for (int i = 0; i < kHandSize; ++i) {
            if (g1->hand.cards[i] < g2->hand.cards[i]) {
                return -1;
            } else if (g1->hand.cards[i] > g2->hand.cards[i]) {
                return 1;
            }
        }
        // Hands are equal
        assert(0);
    }
    
    return 0;
}


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
        String hand_str = ParseWord(&parser);
        Hand hand = StringToHand(hand_str);
        Advance(&parser);  // Space
        u32 bid = ParseNumber(&parser);
        assert(AtEndOfLine(&parser));
        Advance(&parser);  // End of Line
        
        games[game_count] = { hand_str, hand, bid, ClassifyHand(hand) };
        game_count++;
    }
    
    fprintf(stdout, "Game count: %u\n", game_count);
//    for (int i = 0; i < game_count; ++i) {
//        Game g = games[i];
//        fprintf(stdout, "Game %u: %.*s -> %u (%u)\n",
//                i, (int) g.hand_str.size, g.hand_str.start, g.bid, g.type);
//    }
    
    // Sort the hands.
    
    qsort(&games, game_count, sizeof(Game), CardCompare);

//    fprintf(stdout, "Sorted\n");
//    for (int i = 0; i < game_count; ++i) {
//        Game g = games[i];
//        fprintf(stdout, "Game %u: %.*s -> %u (%u)\n",
//                i, (int) g.hand_str.size, g.hand_str.start, g.bid, g.type);
//    }

    u32 rank = 1;
    u32 winnings = 0;
    for (int i = 0; i < game_count; ++i, ++rank) {
        winnings += rank * games[i].bid;
    }

    fprintf(stdout, "Winnings: %u\n", winnings);

    CloseFile(&input_file);
}

