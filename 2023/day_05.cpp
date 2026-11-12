// day_05.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <string.h>
#include <vector>


static const int kMapCount = 7;
static const u64 kEndLimit = (u64) INT64_MAX + 1;


struct SeedRange
{
    u64 start;
    u64 end;
};


struct AlmanacRule
{
    u64 destination;
    SeedRange source;
};


static bool SolveDay05(const File* input_file, bool seed_ranges, i64* answer)
{
    String input = { input_file->data, input_file->size };
    String seeds = ReadLine(&input);
    const String label = CONST_STRING("seeds:");
    if (seeds.size < label.size || memcmp(seeds.start, label.start, label.size)) {
        return false;
    }
    seeds.start += label.size;
    seeds.size -= label.size;

    std::vector<SeedRange> ranges;
    i64 start = 0;
    while (ReadInteger(&seeds, &start)) {
        i64 count = 1;
        if (seed_ranges && !ReadInteger(&seeds, &count)) {
            return false;
        }
        if (start < 0 || count <= 0 || (u64) start + count > kEndLimit) {
            return false;
        }
        ranges.push_back({ (u64) start, (u64) start + count });
    }
    if (ranges.empty()) {
        return false;
    }

    std::vector<AlmanacRule> maps[kMapCount];
    int map = -1;
    const String suffix = CONST_STRING(" map:");
    while (input.size) {
        String line = ReadLine(&input);
        if (!line.size) {
            continue;
        }
        if (line.size >= suffix.size && !memcmp(line.start + line.size - suffix.size, suffix.start, suffix.size)) {
            if (++map >= kMapCount) {
                return false;
            }
            continue;
        }

        i64 destination = 0;
        i64 source = 0;
        i64 count = 0;
        if (map < 0 || !ReadInteger(&line, &destination) || !ReadInteger(&line, &source) || !ReadInteger(&line, &count) ||
            destination < 0 || source < 0 || count <= 0 || (u64) source + count > kEndLimit || (u64) destination + count > kEndLimit) {
            return false;
        }
        maps[map].push_back({ (u64) destination, { (u64) source, (u64) source + count } });
    }
    if (map != kMapCount - 1) {
        return false;
    }

    for (const auto& rules : maps) {
        std::vector<SeedRange> mapped;
        std::vector<SeedRange> pending;
        for (const AlmanacRule& rule : rules) {
            pending.clear();
            for (SeedRange range : ranges) {
                u64 first = range.start > rule.source.start ? range.start : rule.source.start;
                u64 last = range.end < rule.source.end ? range.end : rule.source.end;
                if (first >= last) {
                    pending.push_back(range);
                    continue;
                }

                // Map the overlap and keep the other pieces for the next rule.
                mapped.push_back({ rule.destination + (first - rule.source.start), rule.destination + (last - rule.source.start) });
                if (range.start < first) {
                    pending.push_back({ range.start, first });
                }
                if (last < range.end) {
                    pending.push_back({ last, range.end });
                }
            }
            ranges.swap(pending);
        }
        mapped.insert(mapped.end(), ranges.begin(), ranges.end());
        ranges.swap(mapped);
    }

    u64 lowest = kEndLimit;
    for (SeedRange range : ranges) {
        if (range.start < lowest) {
            lowest = range.start;
        }
    }
    *answer = (i64) lowest;
    return true;
}


bool Day05Part1(const File* input_file, i64* answer)
{
    return SolveDay05(input_file, false, answer);
}


bool Day05Part2(const File* input_file, i64* answer)
{
    return SolveDay05(input_file, true, answer);
}
