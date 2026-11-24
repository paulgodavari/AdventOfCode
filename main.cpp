// main.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"
#include "AdventOfCode-Swift.h"

#include <algorithm>
#include <string_view>
#include <tuple>
#include <vector>

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>


enum class Language
{
    All,
    Cpp,
    Swift,
};


enum class SolveStatus
{
    Answer,
    Unfinished,
    InvalidInput,
};


struct Options
{
    int year;
    int day;
    int part;
    bool use_example;
    const char* input_path;
    Language language;
};


struct Part
{
    int year;
    int day;
    int number;
    bool (*solve)(const File* input_file, i64* answer);
    bool (*solve_test)(const File* input_file, i64* answer) = nullptr;
    Language language = Language::Cpp;
};


static const char kDataDirectory[] = "AdventOfCodeData";
static const Part kCppParts[] = {
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
    { 2023, 13, 1, Day13Part1 },
    { 2023, 13, 2, Day13Part2 },
    { 2023, 14, 1, Day14Part1 },
    { 2023, 14, 2, Day14Part2 },
    { 2023, 15, 1, Day15Part1 },
    { 2023, 15, 2, Day15Part2 },
    { 2023, 16, 1, Day16Part1 },
    { 2023, 16, 2, Day16Part2 },
    { 2023, 18, 1, Day18Part1 },
    { 2023, 19, 1, Day19Part1 },
    { 2023, 19, 2, Day19Part2 },
    { 2023, 21, 1, Day21Part1, Day21Part1Test },
    { 2023, 22, 1, Day22Part1 },
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
    { 2024, 13, 1, Day13Part1_2024 },
    { 2024, 13, 2, Day13Part2_2024 },
    { 2024, 14, 1, Day14Part1_2024, Day14Part1Test_2024 },
    { 2025, 1, 1, Day01Part1_2025 },
    { 2025, 1, 2, Day01Part2_2025 },
    { 2025, 3, 1, Day03Part1_2025 },
};


static void PrintUsage(FILE* output)
{
    fprintf(output, "Usage: aoc [--year YEAR [--day DAY [--part PART]]] [--lang c++|swift] [--test] [--input FILE]\n"
                    "       aoc --help\n"
                    "Without filters, run every registered year, day, part, and language.\n"
                    "--lang selects one language; without it, run all registered implementations.\n"
                    "Run from the project root. --test selects test_input.txt. A regular run selects input.txt.\n"
                    "Part-specific files such as test_input2.txt or input2.txt take priority.\n"
                    "aoc checks results against answers.txt when an answer is available for the selected input.\n"
                    "--input overrides the selected input and skips that check.\n"
                    "It requires a year and day and takes a path relative to AdventOfCodeData/.\n"
                    "ERROR marks a failed run or wrong answer. UNVERIFIED marks a result with no saved answer.\n"
                    "UNFINISHED marks a registered solver that has not been implemented yet.\n");
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
        std::string_view flag = argv[index];
        if (flag == "--test") {
            if (options->use_example) {
                fprintf(stderr, "Repeated option: %s.\n", flag.data());
                return false;
            }

            options->use_example = true;
            continue;
        }

        if (index + 1 == argc || std::string_view(argv[index + 1]).starts_with("--")) {
            fprintf(stderr, "Missing value for %s.\n", flag.data());
            return false;
        }

        std::string_view value = argv[++index];
        if (flag == "--year" && !options->year && ParsePositiveInteger(value.data(), INT_MAX, &options->year)) {
            continue;
        }
        if (flag == "--day" && !options->day && ParsePositiveInteger(value.data(), 25, &options->day)) {
            continue;
        }
        if (flag == "--part" && !options->part && ParsePositiveInteger(value.data(), 2, &options->part)) {
            continue;
        }
        if (flag == "--input" && !options->input_path && IsRelativeInputPath(value.data())) {
            options->input_path = value.data();
            continue;
        }
        if (flag == "--lang" && options->language == Language::All) {
            if (value == "c++") {
                options->language = Language::Cpp;
                continue;
            }
            if (value == "swift") {
                options->language = Language::Swift;
                continue;
            }
        }

        fprintf(stderr, "Invalid or repeated option: %s.\n", flag.data());
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


static std::vector<Part> RegisteredParts()
{
    std::vector<Part> parts;
    for (const Part& part : kCppParts) {
        parts.push_back(part);
    }

    intptr_t swift_part_count = AdventOfCode::swiftPartCount();
    for (intptr_t index = 0; index < swift_part_count; ++index) {
        auto part = AdventOfCode::swiftPart(index);
        parts.push_back({ part.getYear(), part.getDay(), part.getPart(), nullptr, nullptr, Language::Swift });
    }

    std::sort(parts.begin(), parts.end(), [](const Part& lhs, const Part& rhs) {
        return std::tie(lhs.year, lhs.day, lhs.number, lhs.language) < std::tie(rhs.year, rhs.day, rhs.number, rhs.language);
    });
    return parts;
}


static void PrintPartPrefix(const Part& part)
{
    const char* language = part.language == Language::Swift ? "Swift" : "C++";
    printf("%d Day %d part %d (%s): ", part.year, part.day, part.number, language);
}


static SolveStatus SolvePart(const Part& part, const Options& options, const File* input_file, i64* answer)
{
    if (part.language == Language::Swift) {
        auto result = AdventOfCode::solveSwiftPart(part.year, part.day, part.number, (const u8*) input_file->data,
                                                 (intptr_t) input_file->size, options.use_example);
        switch (result) {
            case AdventOfCode::SwiftSolveResult::answer:
                *answer = result.getAnswer();
                return SolveStatus::Answer;
            case AdventOfCode::SwiftSolveResult::unfinished:
                return SolveStatus::Unfinished;
            default:
                return SolveStatus::InvalidInput;
        }
    }

    auto solve = options.use_example && part.solve_test ? part.solve_test : part.solve;
    return solve(input_file, answer) ? SolveStatus::Answer : SolveStatus::InvalidInput;
}


static bool RunPart(const Part& part, const Options& options)
{
    char path[PATH_MAX];
    int length = FindInputPath(part, options, path, sizeof(path));
    if (length < 0 || (size_t) length >= sizeof(path)) {
        PrintPartPrefix(part);
        printf("ERROR could not form the input path\n");
        return false;
    }

    File input_file = ReadFile(path);
    if (!input_file.handle) {
        PrintPartPrefix(part);
        printf("ERROR could not read input\n");
        return false;
    }

    i64 answer = 0;
    u64 start_time = TimeNow();
    SolveStatus status = SolvePart(part, options, &input_file, &answer);
    f64 elapsed_ms = MillisecondsSince(start_time);
    CloseFile(&input_file);

    PrintPartPrefix(part);
    if (status == SolveStatus::Unfinished) {
        printf("UNFINISHED %.3f ms\n", elapsed_ms);
        return true;
    }
    if (status == SolveStatus::InvalidInput) {
        printf("ERROR %.3f ms (invalid input or answer out of range)\n", elapsed_ms);
        fprintf(stderr, "Input: %s\n", path);
        return false;
    }

    printf("%" PRId64 " %.3f ms", answer, elapsed_ms);
    i64 expected = 0;
    bool success = true;
    if (!options.input_path && ReadSavedAnswer(part, options.use_example, &expected)) {
        success = answer == expected;
        if (!success) {
            printf(" ERROR expected=%" PRId64, expected);
        }
    } else {
        printf(" UNVERIFIED");
    }
    putchar('\n');
    return success;
}


int main(int argc, char* argv[])
{
    if (argc == 2 && std::string_view(argv[1]) == "--help") {
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
    for (const Part& part : RegisteredParts()) {
        if ((options.year && options.year != part.year) ||
            (options.day && options.day != part.day) ||
            (options.part && options.part != part.number) ||
            (options.language != Language::All && options.language != part.language)) {
            continue;
        }

        found = true;
        if (!RunPart(part, options)) {
            success = false;
        }
    }

    if (!found) {
        fprintf(stderr, "No registered implementations match the selected year, day, part, and language.\n");
        return 1;
    }

    return success ? 0 : 1;
}
