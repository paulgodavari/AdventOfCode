// main.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <ctype.h>
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
    bool use_example;
    const char* input_path;
};


struct Part
{
    int year;
    int day;
    int number;
    bool (*solve)(const File* input_file, i64* answer);
    bool (*solve_test)(const File* input_file, i64* answer) = nullptr;
};


static const char kDataDirectory[] = "AdventOfCodeData";
static const Part kParts[] = {
    { 2023, 1, 1, Day01Part1 },
    { 2023, 1, 2, Day01Part2 },
    { 2023, 2, 1, Day02Part1 },
    { 2023, 2, 2, Day02Part2 },
    { 2023, 3, 1, Day03Part1 },
    { 2023, 3, 2, Day03Part2 },
    { 2023, 4, 1, Day04Part1 },
    { 2023, 4, 2, Day04Part2 },
    { 2023, 5, 1, Day05Part1 },
    { 2023, 5, 2, Day05Part2 },
    { 2023, 6, 1, Day06Part1 },
    { 2023, 6, 2, Day06Part2 },
    { 2023, 7, 1, Day07Part1 },
    { 2023, 7, 2, Day07Part2 },
    { 2023, 8, 1, Day08Part1 },
    { 2023, 8, 2, Day08Part2 },
    { 2023, 9, 1, Day09Part1 },
    { 2023, 9, 2, Day09Part2 },
    { 2023, 10, 1, Day10Part1 },
    { 2023, 10, 2, Day10Part2 },
    { 2023, 11, 1, Day11Part1 },
    { 2023, 11, 2, Day11Part2, Day11Part2Test },
    { 2023, 12, 1, Day12Part1 },
    { 2023, 12, 2, nullptr },
    { 2023, 13, 1, Day13Part1 },
    { 2023, 13, 2, Day13Part2 },
    { 2023, 14, 1, Day14Part1 },
    { 2023, 14, 2, Day14Part2 },
    { 2023, 15, 1, Day15Part1 },
    { 2023, 15, 2, Day15Part2 },
    { 2023, 16, 1, Day16Part1 },
    { 2023, 16, 2, Day16Part2 },
    { 2023, 17, 1, nullptr },
    { 2023, 17, 2, nullptr },
    { 2023, 18, 1, Day18Part1 },
    { 2023, 18, 2, nullptr },
    { 2023, 19, 1, Day19Part1 },
    { 2023, 19, 2, Day19Part2 },
    { 2023, 20, 1, nullptr },
    { 2023, 20, 2, nullptr },
    { 2023, 21, 1, Day21Part1, Day21Part1Test },
    { 2023, 21, 2, nullptr },
    { 2023, 22, 1, Day22Part1 },
    { 2023, 22, 2, nullptr },
    { 2023, 23, 1, nullptr },
    { 2023, 23, 2, nullptr },
    { 2023, 24, 1, nullptr },
    { 2023, 24, 2, nullptr },
    { 2023, 25, 1, nullptr },
    { 2023, 25, 2, nullptr },
    { 2024, 1, 1, Day01Part1_2024 },
    { 2024, 1, 2, Day01Part2_2024 },
    { 2024, 2, 1, Day02Part1_2024 },
    { 2024, 2, 2, Day02Part2_2024 },
    { 2024, 3, 1, Day03Part1_2024 },
    { 2024, 3, 2, Day03Part2_2024 },
    { 2024, 4, 1, Day04Part1_2024 },
    { 2024, 4, 2, Day04Part2_2024 },
    { 2024, 5, 1, Day05Part1_2024 },
    { 2024, 5, 2, Day05Part2_2024 },
    { 2024, 6, 1, Day06Part1_2024 },
    { 2024, 6, 2, Day06Part2_2024 },
    { 2024, 7, 1, Day07Part1_2024 },
    { 2024, 7, 2, Day07Part2_2024 },
    { 2024, 8, 1, Day08Part1_2024 },
    { 2024, 8, 2, Day08Part2_2024 },
    { 2024, 9, 1, Day09Part1_2024 },
    { 2024, 9, 2, Day09Part2_2024 },
    { 2024, 10, 1, Day10Part1_2024 },
    { 2024, 10, 2, Day10Part2_2024 },
    { 2024, 11, 1, Day11Part1_2024 },
    { 2024, 11, 2, Day11Part2_2024 },
    { 2024, 12, 1, Day12Part1_2024 },
    { 2024, 12, 2, nullptr },
    { 2024, 13, 1, Day13Part1_2024 },
    { 2024, 13, 2, Day13Part2_2024 },
    { 2024, 14, 1, Day14Part1_2024, Day14Part1Test_2024 },
    { 2024, 14, 2, nullptr },
    { 2025, 1, 1, Day01Part1_2025 },
    { 2025, 1, 2, Day01Part2_2025 },
    { 2025, 2, 1, nullptr },
    { 2025, 2, 2, nullptr },
    { 2025, 3, 1, Day03Part1_2025 },
    { 2025, 3, 2, nullptr },
};


static void PrintUsage(FILE* output)
{
    fprintf(output, "Usage: aoc [--year YEAR [--day DAY [--part PART]]] [--test] [--input FILE]\n"
                    "       aoc --help\n"
                    "Run from the project root. --test selects test_input.txt. A regular run selects input.txt.\n"
                    "Part-specific files such as test_input2.txt or input2.txt take priority.\n"
                    "aoc checks results against answers.txt when an answer is available for the selected input.\n"
                    "--input overrides the selected input and skips that check.\n"
                    "It requires a year and day and takes a path relative to AdventOfCodeData/.\n");
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
    for (int index = 1; index < argc; ++index) {
        const char* flag = argv[index];
        if (strcmp(flag, "--test") == 0) {
            if (options->use_example) {
                fprintf(stderr, "Repeated option: %s.\n", flag);
                return false;
            }

            options->use_example = true;
            continue;
        }

        if (index + 1 == argc || strncmp(argv[index + 1], "--", 2) == 0) {
            fprintf(stderr, "Missing value for %s.\n", flag);
            return false;
        }

        const char* value = argv[++index];
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


static bool ReadSavedAnswer(const Part& part, bool use_example, i64* answer)
{
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%d/%02d/answers.txt", kDataDirectory, part.year, part.day);
    FILE* file = fopen(path, "r");
    if (!file) {
        return false;
    }

    char* line = nullptr;
    size_t capacity = 0;
    bool found = true;
    for (int number = 0; number < part.number; ++number) {
        if (getline(&line, &capacity, file) < 0) {
            found = false;
            break;
        }
    }
    fclose(file);

    // Each line starts with the example answer and can end with the real answer.
    const char* start = line;
    int column_count = use_example ? 1 : 2;
    intmax_t value = 0;
    for (int column = 0; found && column < column_count; ++column) {
        errno = 0;
        char* end = nullptr;
        value = strtoimax(start, &end, 10);
        found = end != start && errno == 0 && value >= INT64_MIN && value <= INT64_MAX && (*end == '\0' || isspace((unsigned char) *end));
        start = end;
    }

    free(line);
    if (found) {
        *answer = (i64) value;
    }
    return found;
}


static int FindInputPath(const Part& part, const Options& options, char* path, size_t capacity)
{
    if (options.input_path) {
        return snprintf(path, capacity, "%s/%s", kDataDirectory, options.input_path);
    }

    char part_filename[32];
    char legacy_part_filename[32];
    snprintf(part_filename, sizeof(part_filename), options.use_example ? "test_input%d.txt" : "input%d.txt", part.number);
    snprintf(legacy_part_filename, sizeof(legacy_part_filename), options.use_example ? "example-%d.txt" : "%d.txt", part.number);

    // Try the old names too so the runner can read days whose files still use them.
    const char* test_filenames[] = { part_filename, "test_input.txt", "example.txt", legacy_part_filename, "example-1.txt", nullptr };
    const char* real_filenames[] = { part_filename, legacy_part_filename, "input.txt", nullptr };
    const char* const* filenames = options.use_example ? test_filenames : real_filenames;
    for (int index = 0; filenames[index]; ++index) {
        int length = snprintf(path, capacity, "%s/%d/%02d/%s", kDataDirectory, part.year, part.day, filenames[index]);
        struct stat file_info = {};
        if (length < 0 || (size_t) length >= capacity || stat(path, &file_info) == 0 || errno != ENOENT) {
            return length;
        }
    }

    const char* shared_filename = options.use_example ? "test_input.txt" : "input.txt";
    return snprintf(path, capacity, "%s/%d/%02d/%s", kDataDirectory, part.year, part.day, shared_filename);
}


static bool RunPart(const Part& part, const Options& options)
{
    if (!part.solve) {
        printf("%d Day %02d Part %d: unfinished\n", part.year, part.day, part.number);
        return options.part == 0;
    }

    char path[PATH_MAX];
    int length = FindInputPath(part, options, path, sizeof(path));
    if (length < 0 || (size_t) length >= sizeof(path)) {
        fprintf(stderr, "Could not form the input path.\n");
        return false;
    }

    File input_file = ReadFile(path);
    if (!input_file.handle) {
        return false;
    }

    i64 answer = 0;
    auto solve = options.use_example && part.solve_test ? part.solve_test : part.solve;
    bool solved = solve(&input_file, &answer);
    CloseFile(&input_file);
    if (!solved) {
        fprintf(stderr, "%d Day %02d Part %d: invalid input or answer out of range in %s.\n", part.year, part.day, part.number, path);
        return false;
    }

    printf("%d Day %02d Part %d: %" PRId64, part.year, part.day, part.number, answer);
    i64 expected = 0;
    bool success = true;
    if (!options.input_path && ReadSavedAnswer(part, options.use_example, &expected)) {
        success = answer == expected;
        if (success) {
            printf(" (matches saved answer)");
        } else {
            printf(" (expected %" PRId64 ")", expected);
        }
    }
    putchar('\n');
    return success;
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
        if (!RunPart(part, options)) {
            success = false;
        }
    }

    if (!found) {
        fprintf(stderr, "No registered parts match the selected year, day, and part.\n");
        return 1;
    }

    return success ? 0 : 1;
}
