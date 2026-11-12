// day_15.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <ctype.h>
#include <string.h>


static const int kBoxCount = 256;


struct Lens
{
    Lens* next;
    String label;
    i64 focal_length;
};


static u32 ComputeHash(String word)
{
    u32 hash = 0;
    for (size_t index = 0; index < word.size; ++index) {
        hash = (hash + (u8) word.start[index]) * 17 % kBoxCount;
    }
    return hash;
}


static bool ProcessStep(String step, Lens** boxes)
{
    size_t index = 0;
    while (index < step.size && isalnum((unsigned char) step.start[index])) {
        ++index;
    }
    if (!index || index == step.size) {
        return false;
    }

    String label = { step.start, index };
    char operation = step.start[index++];
    Lens** link = &boxes[ComputeHash(label)];
    while (*link && !((*link)->label == label)) {
        link = &(*link)->next;
    }

    if (operation == '-' && index == step.size) {
        if (*link) {
            Lens* removed = *link;
            *link = removed->next;
            delete removed;
        }
        return true;
    }

    String length = { step.start + index, step.size - index };
    i64 focal_length = 0;
    if (operation != '=' || !ReadInteger(&length, &focal_length) || length.size || focal_length < 0) {
        return false;
    }

    if (*link) {
        (*link)->focal_length = focal_length;
    } else {
        *link = new Lens { nullptr, label, focal_length };
    }
    return true;
}


static bool SolveDay15(const File* input_file, bool focus, i64* answer)
{
    String input = { input_file->data, input_file->size };
    Lens* boxes[kBoxCount] = {};
    i64 total = 0;
    bool success = true;
    while (success && input.size) {
        String line = ReadLine(&input);
        while (success && line.size) {
            const char* comma = (const char*) memchr(line.start, ',', line.size);
            size_t size = comma ? (size_t) (comma - line.start) : line.size;
            String step = { line.start, size };
            if (focus) {
                success = ProcessStep(step, boxes);
            } else {
                success = !__builtin_add_overflow(total, (i64) ComputeHash(step), &total);
            }
            size += comma != nullptr;
            line.start += size;
            line.size -= size;
        }
    }

    for (int box = 0; box < kBoxCount; ++box) {
        i64 slot = 1;
        while (Lens* lens = boxes[box]) {
            i64 power = 0;
            if (success) {
                success = !__builtin_mul_overflow(box + 1, slot, &power) &&
                          !__builtin_mul_overflow(power, lens->focal_length, &power) &&
                          !__builtin_add_overflow(total, power, &total);
            }
            boxes[box] = lens->next;
            delete lens;
            ++slot;
        }
    }

    *answer = total;
    return success;
}


bool Day15Part1(const File* input_file, i64* answer)
{
    return SolveDay15(input_file, false, answer);
}


bool Day15Part2(const File* input_file, i64* answer)
{
    return SolveDay15(input_file, true, answer);
}
