// day_07.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <ctype.h>
#include <string.h>
#include <vector>


static const int kHandSize = 5;
static const int kCardCount = 13;


enum HandType
{
    kHandTypeHighCard,
    kHandTypeOnePair,
    kHandTypeTwoPair,
    kHandTypeThree,
    kHandTypeFullHouse,
    kHandTypeFour,
    kHandTypeFive,
};


struct CamelHand
{
    int cards[kHandSize];
    i64 bid;
    HandType type;
};


static HandType ClassifyHand(const CamelHand& hand, bool jokers)
{
    int counts[kCardCount] = {};
    for (int card : hand.cards) {
        ++counts[card];
    }

    int first = 0;
    int second = 0;
    for (int card = jokers ? 1 : 0; card < kCardCount; ++card) {
        if (counts[card] > first) {
            second = first;
            first = counts[card];
        } else if (counts[card] > second) {
            second = counts[card];
        }
    }
    if (jokers) {
        first += counts[0];
    }

    if (first == 5) return kHandTypeFive;
    if (first == 4) return kHandTypeFour;
    if (first == 3) return second == 2 ? kHandTypeFullHouse : kHandTypeThree;
    if (first == 2) return second == 2 ? kHandTypeTwoPair : kHandTypeOnePair;
    return kHandTypeHighCard;
}


static int CompareHands(const void* lhs, const void* rhs)
{
    const CamelHand& first = *(const CamelHand*) lhs;
    const CamelHand& second = *(const CamelHand*) rhs;
    if (first.type != second.type) {
        return first.type - second.type;
    }
    for (int index = 0; index < kHandSize; ++index) {
        if (first.cards[index] != second.cards[index]) {
            return first.cards[index] - second.cards[index];
        }
    }
    return 0;
}


static bool SolveDay07(const File* input_file, bool jokers, i64* answer)
{
    const char* ranks = jokers ? "J23456789TQKA" : "23456789TJQKA";
    String input = { input_file->data, input_file->size };
    std::vector<CamelHand> hands;
    while (input.size) {
        String line = ReadLine(&input);
        if (line.size <= kHandSize || (line.start[kHandSize] != ' ' && line.start[kHandSize] != '\t')) {
            return false;
        }

        CamelHand hand = {};
        for (int index = 0; index < kHandSize; ++index) {
            int card = toupper((unsigned char) line.start[index]);
            const char* rank = strchr(ranks, card);
            if (!rank || !*rank) {
                return false;
            }
            hand.cards[index] = (int) (rank - ranks);
        }
        line.start += kHandSize;
        line.size -= kHandSize;
        if (!ReadInteger(&line, &hand.bid) || hand.bid < 0) {
            return false;
        }

        hand.type = ClassifyHand(hand, jokers);
        hands.push_back(hand);
    }

    if (!hands.empty()) {
        qsort(hands.data(), hands.size(), sizeof(CamelHand), CompareHands);
    }

    i64 total = 0;
    for (size_t index = 0; index < hands.size(); ++index) {
        if ((u64) hands[index].bid > (u64) (INT64_MAX - total) / (index + 1)) {
            return false;
        }
        total += (i64) ((index + 1) * (u64) hands[index].bid);
    }
    *answer = total;
    return true;
}


bool Day07Part1(const File* input_file, i64* answer)
{
    return SolveDay07(input_file, false, answer);
}


bool Day07Part2(const File* input_file, i64* answer)
{
    return SolveDay07(input_file, true, answer);
}
