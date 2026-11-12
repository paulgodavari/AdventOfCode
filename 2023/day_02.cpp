// day_02.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <ctype.h>
#include <string.h>


struct CubeGame
{
    i64 id;
    i64 red;
    i64 green;
    i64 blue;
};


static const String kRed = CONST_STRING("red");
static const String kGreen = CONST_STRING("green");
static const String kBlue = CONST_STRING("blue");

static const i64 kRedLimit = 12;
static const i64 kGreenLimit = 13;
static const i64 kBlueLimit = 14;


static void SkipSpace(ParseState* parser)
{
    while (!AtEndOfFile(parser) && isspace((unsigned char) parser->data[parser->offset])) {
        Advance(parser);
    }
}


static bool ParseCount(ParseState* parser, i64* count)
{
    SkipSpace(parser);
    i32 start = parser->offset;
    i64 value = 0;
    while (!AtEndOfFile(parser)) {
        char character = parser->data[parser->offset];
        if (character < '0' || character > '9') {
            break;
        }

        i64 digit = character - '0';
        if (value > (INT64_MAX - digit) / 10) {
            return false;
        }

        value = value * 10 + digit;
        Advance(parser);
    }

    if (parser->offset == start) {
        return false;
    }
    *count = value;
    return true;
}


static bool ParseLine(ParseState* parser, CubeGame* game)
{
    SkipSpace(parser);
    if (!ConsumeString(parser, CONST_STRING("Game")) || !ParseCount(parser, &game->id)) {
        return false;
    }

    SkipSpace(parser);
    if (!ConsumeString(parser, CONST_STRING(":"))) {
        return false;
    }

    // Keep the largest count shown for each color.
    while (true) {
        i64 count = 0;
        if (!ParseCount(parser, &count)) {
            return false;
        }

        SkipSpace(parser);
        if (AtEndOfFile(parser) || parser->data[parser->offset] < 'a' || parser->data[parser->offset] > 'z') {
            return false;
        }

        String color = ParseWord(parser);
        i64* maximum = nullptr;
        if (color == kRed) {
            maximum = &game->red;
        } else if (color == kGreen) {
            maximum = &game->green;
        } else if (color == kBlue) {
            maximum = &game->blue;
        } else {
            return false;
        }
        if (count > *maximum) {
            *maximum = count;
        }

        SkipSpace(parser);
        if (AtEndOfFile(parser)) {
            return true;
        }

        char separator = parser->data[parser->offset];
        if (separator != ',' && separator != ';') {
            return false;
        }
        Advance(parser);
    }
}


static bool FindPower(const CubeGame& game, i64* power)
{
    if (game.red == 0 || game.green == 0 || game.blue == 0) {
        *power = 0;
        return true;
    }

    if (game.red > INT64_MAX / game.green) {
        return false;
    }
    i64 red_green = game.red * game.green;
    if (red_green > INT64_MAX / game.blue) {
        return false;
    }

    *power = red_green * game.blue;
    return true;
}


static bool SolveDay02(const File* input_file, bool sum_powers, i64* answer)
{
    i64 total = 0;
    size_t offset = 0;
    while (offset < input_file->size) {
        char* line = input_file->data + offset;
        size_t remaining = input_file->size - offset;
        const char* line_end = (const char*) memchr(line, '\n', remaining);
        size_t line_size = line_end ? (size_t) (line_end - line) : remaining;
        if (line_size > INT32_MAX) {
            return false;
        }

        ParseState parser = { line, line_size, 0 };
        CubeGame game = {};
        if (!ParseLine(&parser, &game)) {
            return false;
        }

        i64 value = 0;
        if (sum_powers) {
            if (!FindPower(game, &value)) {
                return false;
            }
        } else if (game.red <= kRedLimit && game.green <= kGreenLimit && game.blue <= kBlueLimit) {
            value = game.id;
        }

        if (total > INT64_MAX - value) {
            return false;
        }
        total += value;
        offset += line_size + (line_end ? 1 : 0);
    }

    *answer = total;
    return true;
}


bool Day02Part1(const File* input_file, i64* answer)
{
    return SolveDay02(input_file, false, answer);
}


bool Day02Part2(const File* input_file, i64* answer)
{
    return SolveDay02(input_file, true, answer);
}
