// day_05_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <bitset>
#include <vector>


static const int kPageCount = 100;


static bool ReadPage(String* input, int* page)
{
    i64 value = 0;
    if (!ReadInteger(input, &value) || value < 0 || value >= kPageCount) {
        return false;
    }
    *page = (int) value;
    return true;
}


static bool SolveDay05(const File* input_file, bool reorder, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::bitset<kPageCount> rules[kPageCount];
    while (input.size) {
        String line = ReadLine(&input);
        if (!line.size) {
            break;
        }

        int first = 0;
        int second = 0;
        if (!ReadPage(&line, &first) || !ConsumeString(&line, CONST_STRING("|")) || !ReadPage(&line, &second) || line.size) {
            return false;
        }
        rules[first].set(second);
    }

    i64 total = 0;
    while (input.size) {
        String line = ReadLine(&input);
        std::vector<int> pages;
        std::bitset<kPageCount> seen;
        bool ordered = true;
        for (;;) {
            int page = 0;
            if (!ReadPage(&line, &page) || seen.test(page)) {
                return false;
            }
            ordered = ordered && (rules[page] & seen).none();
            seen.set(page);
            pages.push_back(page);

            if (!line.size) {
                break;
            }
            if (!ConsumeString(&line, CONST_STRING(","))) {
                return false;
            }
        }

        if (!reorder && ordered) {
            total += pages[pages.size() / 2];
        } else if (reorder && !ordered) {
            size_t middle = pages.size() / 2;
            for (size_t index = 0; index <= middle; ++index) {
                size_t first = 0;
                for (; first < pages.size(); ++first) {
                    bool has_predecessor = false;
                    for (int page : pages) {
                        has_predecessor = has_predecessor || rules[page].test(pages[first]);
                    }
                    if (!has_predecessor) {
                        break;
                    }
                }
                if (first == pages.size()) {
                    return false;
                }
                if (index == middle) {
                    total += pages[first];
                }
                pages.erase(pages.begin() + first);
            }
        }
    }
    *answer = total;
    return true;
}


bool Day05Part1_2024(const File* input_file, i64* answer)
{
    return SolveDay05(input_file, false, answer);
}


bool Day05Part2_2024(const File* input_file, i64* answer)
{
    return SolveDay05(input_file, true, answer);
}
