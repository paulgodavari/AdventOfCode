// day_09_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


struct DiskSpan
{
    i64 start;
    i64 length;
};


struct DiskFile
{
    DiskSpan span;
    i64 id;
};


static bool ReadDiskMap(const File* file, String* map)
{
    String input = { file->data, file->size };
    *map = ReadLine(&input);
    if (!map->size || map->size > (u64) INT64_MAX / 9 || input.size) {
        return false;
    }
    for (size_t index = 0; index < map->size; ++index) {
        if (map->start[index] < '0' || map->start[index] > '9') {
            return false;
        }
    }
    return true;
}


static bool AddChecksum(i64 index, i64 id, i64* total)
{
    i64 value = 0;
    return !__builtin_mul_overflow(index, id, &value) && !__builtin_add_overflow(*total, value, total);
}


bool Day09Part1_2024(const File* input_file, i64* answer)
{
    String map = {};
    if (!ReadDiskMap(input_file, &map)) {
        return false;
    }

    std::vector<i64> blocks;
    for (size_t index = 0; index < map.size; ++index) {
        i64 id = index % 2 ? -1 : (i64) (index / 2);
        blocks.insert(blocks.end(), map.start[index] - '0', id);
    }

    i64 total = 0;
    size_t last = blocks.size();
    for (size_t first = 0; first < last; ++first) {
        i64 id = blocks[first];
        if (id < 0) {
            do {
                --last;
            } while (last > first && blocks[last] < 0);
            if (last == first) {
                break;
            }
            id = blocks[last];
        }
        if (!AddChecksum((i64) first, id, &total)) {
            return false;
        }
    }
    *answer = total;
    return true;
}


bool Day09Part2_2024(const File* input_file, i64* answer)
{
    String map = {};
    if (!ReadDiskMap(input_file, &map)) {
        return false;
    }

    std::vector<DiskFile> files;
    std::vector<DiskSpan> spaces;
    i64 start = 0;
    for (size_t index = 0; index < map.size; ++index) {
        i64 length = map.start[index] - '0';
        if (length) {
            if (index % 2 == 0) {
                files.push_back({ { start, length }, (i64) (index / 2) });
            } else if (!spaces.empty() && spaces.back().start + spaces.back().length == start) {
                spaces.back().length += length;
            } else {
                spaces.push_back({ start, length });
            }
        }
        start += length;
    }

    i64 total = 0;
    for (size_t index = files.size(); index;) {
        DiskFile file = files[--index];
        for (DiskSpan& space : spaces) {
            if (space.start >= file.span.start) {
                break;
            }
            if (space.length >= file.span.length) {
                file.span.start = space.start;
                space.start += file.span.length;
                space.length -= file.span.length;
                break;
            }
        }
        for (i64 block = 0; block < file.span.length; ++block) {
            if (!AddChecksum(file.span.start + block, file.id, &total)) {
                return false;
            }
        }
    }
    *answer = total;
    return true;
}
