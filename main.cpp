// main.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>


struct Options
{
    int year;
    int day;
    int part;
    const char* input_path;
};


struct Part
{
    int year;
    int day;
    int number;
    bool (*solve)(const File* input_file, i64* answer);
};


static const char kDataDirectory[] = "AdventOfCodeData";
static const Part kParts[] = {
    { 2023, 6, 1, Day06Part1 },
    { 2023, 6, 2, Day06Part2 },
};


static void PrintUsage(FILE* output)
{
    fprintf(output, "Usage: aoc [--year YEAR [--day DAY [--part PART]]] [--input FILE]\n"
                    "       aoc --help\n"
                    "Run from the project root. --input requires a year and day and takes a path relative to AdventOfCodeData/.\n");
}


static bool ParsePositiveInteger(const char* text, int maximum, int* value)
{
    if (*text < '0' || *text > '9') {
        return false;
    }

    char* end = nullptr;
    long number = strtol(text, &end, 10);
    if (*end || number < 1 || number > maximum) {
        return false;
    }

    *value = (int) number;
    return true;
}


static bool IsRelativeInputPath(const char* path)
{
    if (!*path || *path == '/') {
        return false;
    }

    for (const char* component = path; component;) {
        if (component[0] == '.' && component[1] == '.' && (component[2] == '/' || component[2] == '\0')) {
            return false;
        }

        const char* slash = strchr(component, '/');
        component = slash ? slash + 1 : nullptr;
    }

    return true;
}


static bool ParseOptions(int argc, char* argv[], Options* options)
{
    for (int index = 1; index < argc; index += 2) {
        const char* flag = argv[index];
        if (index + 1 == argc || strncmp(argv[index + 1], "--", 2) == 0) {
            fprintf(stderr, "Missing value for %s.\n", flag);
            return false;
        }

        const char* value = argv[index + 1];
        if (strcmp(flag, "--year") == 0 && !options->year && ParsePositiveInteger(value, INT_MAX, &options->year)) {
            continue;
        }
        if (strcmp(flag, "--day") == 0 && !options->day && ParsePositiveInteger(value, 25, &options->day)) {
            continue;
        }
        if (strcmp(flag, "--part") == 0 && !options->part && ParsePositiveInteger(value, 2, &options->part)) {
            continue;
        }
        if (strcmp(flag, "--input") == 0 && !options->input_path && IsRelativeInputPath(value)) {
            options->input_path = value;
            continue;
        }

        fprintf(stderr, "Invalid or repeated option: %s.\n", flag);
        return false;
    }

    return (!options->day || options->year) && (!(options->part || options->input_path) || options->day);
}


static bool RunPart(const Part& part, const char* input_path)
{
    char path[PATH_MAX];
    int length = 0;
    if (input_path) {
        length = snprintf(path, sizeof(path), "%s/%s", kDataDirectory, input_path);
    } else {
        length = snprintf(path, sizeof(path), "%s/%d/%02d/%d.txt", kDataDirectory, part.year, part.day, part.number);
        struct stat file_info = {};
        if (stat(path, &file_info) == -1 && errno == ENOENT) {
            length = snprintf(path, sizeof(path), "%s/%d/%02d/input.txt", kDataDirectory, part.year, part.day);
        }
    }

    if (length < 0 || (size_t) length >= sizeof(path)) {
        fprintf(stderr, "Could not form the input path.\n");
        return false;
    }

    File input_file = ReadFile(path);
    if (!input_file.handle) {
        return false;
    }

    i64 answer = 0;
    bool solved = part.solve(&input_file, &answer);
    CloseFile(&input_file);
    if (!solved) {
        fprintf(stderr, "%d Day %02d Part %d: invalid input or answer out of range in %s.\n", part.year, part.day, part.number, path);
        return false;
    }

    printf("%d Day %02d Part %d: %" PRId64 "\n", part.year, part.day, part.number, answer);
    return true;
}


int main(int argc, char* argv[])
{
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        PrintUsage(stdout);
        return 0;
    }

    Options options = {};
    if (!ParseOptions(argc, argv, &options)) {
        PrintUsage(stderr);
        return 2;
    }

    bool found = false;
    bool success = true;
    for (const Part& part : kParts) {
        if ((options.year && options.year != part.year) ||
            (options.day && options.day != part.day) ||
            (options.part && options.part != part.number)) {
            continue;
        }

        found = true;
        if (!RunPart(part, options.input_path)) {
            success = false;
        }
    }

    if (!found) {
        fprintf(stderr, "No registered parts match the selected year, day, and part.\n");
        return 1;
    }

    return success ? 0 : 1;
}
