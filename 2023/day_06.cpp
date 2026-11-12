// day_06.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <ctype.h>
#include <string.h>


static void SkipRaceSpace(ParseState* row)
{
    while (!AtEndOfFile(row) && isspace((unsigned char) row->data[row->offset])) {
        Advance(row);
    }
}


static bool AppendRaceNumber(ParseState* row, u64* value)
{
    i32 start = row->offset;
    while (!AtEndOfFile(row)) {
        char character = row->data[row->offset];
        if (character < '0' || character > '9') {
            break;
        }

        u64 digit = character - '0';
        if (*value > (UINT64_MAX - digit) / 10) {
            return false;
        }

        *value = *value * 10 + digit;
        Advance(row);
    }

    bool has_digits = row->offset != start;
    SkipRaceSpace(row);
    return has_digits;
}


static u64 CountWinningHolds(u64 time, u64 record)
{
    u64 first = 1;
    u64 last = time / 2;
    if (last == 0 || last <= record / (time - last)) {
        return 0;
    }

    // Distance grows up to the midpoint. Find the first hold that beats the record.
    while (first < last) {
        u64 hold = first + (last - first) / 2;
        if (hold > record / (time - hold)) {
            last = hold;
        } else {
            first = hold + 1;
        }
    }

    // A hold of time - first covers the same distance as a hold of first.
    return time - 2 * first + 1;
}


static bool SolveDay06(const File* input_file, bool concatenate, i64* answer)
{
    if (input_file->size > INT32_MAX) {
        return false;
    }

    char* line_end = (char*) memchr(input_file->data, '\n', input_file->size);
    if (!line_end) {
        return false;
    }

    size_t time_size = line_end - input_file->data;
    ParseState times = { input_file->data, time_size, 0 };
    ParseState distances = { line_end + 1, input_file->size - time_size - 1, 0 };
    if (!ConsumeString(&times, CONST_STRING("Time:")) || !ConsumeString(&distances, CONST_STRING("Distance:"))) {
        return false;
    }

    SkipRaceSpace(&times);
    SkipRaceSpace(&distances);

    const u64 kMaxAnswer = INT64_MAX;
    u64 time = 0;
    u64 distance = 0;
    u64 result = 1;
    do {
        if (!AppendRaceNumber(&times, &time) || !AppendRaceNumber(&distances, &distance)) {
            return false;
        }

        if (!concatenate) {
            u64 wins = CountWinningHolds(time, distance);
            if (wins && result > kMaxAnswer / wins) {
                // Keep the overflow until a race with zero wins can reset the product.
                result = kMaxAnswer + 1;
            } else {
                result *= wins;
            }
            time = 0;
            distance = 0;
        }
    } while (!AtEndOfFile(&times));

    if (!AtEndOfFile(&distances)) {
        return false;
    }
    if (concatenate) {
        result = CountWinningHolds(time, distance);
    }
    if (result > kMaxAnswer) {
        return false;
    }

    *answer = (i64) result;
    return true;
}


bool Day06Part1(const File* input_file, i64* answer)
{
    return SolveDay06(input_file, false, answer);
}


bool Day06Part2(const File* input_file, i64* answer)
{
    return SolveDay06(input_file, true, answer);
}
