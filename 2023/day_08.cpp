// day_08.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


static const int kAlphabetSize = 36;
static const int kNodeCount = kAlphabetSize * kAlphabetSize * kAlphabetSize;


struct DesertNode
{
    int left;
    int right;
};


struct DesertMap
{
    String directions;
    std::vector<DesertNode> nodes;
};


struct DesertPosition
{
    int node;
    size_t direction;
};


struct DesertCycle
{
    i64 prefix;
    i64 period;
    std::vector<i64> finishes;
};


static int NodeDigit(char character)
{
    if (character >= 'A' && character <= 'Z') {
        return character - 'A';
    }
    if (character >= '0' && character <= '9') {
        return 26 + character - '0';
    }
    return -1;
}


static int ReadNode(String* line)
{
    size_t index = 0;
    while (index < line->size && NodeDigit(line->start[index]) < 0) {
        ++index;
    }

    int node = 0;
    int count = 0;
    for (; index < line->size && NodeDigit(line->start[index]) >= 0; ++index) {
        if (++count > 3) {
            return -1;
        }
        node = node * kAlphabetSize + NodeDigit(line->start[index]);
    }

    line->start += index;
    line->size -= index;
    return count == 3 ? node : -1;
}


static bool ReadDesertMap(const File* input_file, DesertMap* map)
{
    String input = { input_file->data, input_file->size };
    map->directions = ReadLine(&input);
    if (!map->directions.size) {
        return false;
    }
    for (size_t index = 0; index < map->directions.size; ++index) {
        char direction = map->directions.start[index];
        if (direction != 'L' && direction != 'R') {
            return false;
        }
    }

    map->nodes.assign(kNodeCount, { -1, -1 });
    while (input.size) {
        String line = ReadLine(&input);
        if (!line.size) {
            continue;
        }
        int node = ReadNode(&line);
        int left = ReadNode(&line);
        int right = ReadNode(&line);
        if (node < 0 || left < 0 || right < 0 || map->nodes[node].left >= 0) {
            return false;
        }
        map->nodes[node] = { left, right };
    }

    return true;
}


static DesertPosition NextPosition(const DesertMap& map, DesertPosition position)
{
    if (position.node < 0) {
        return position;
    }
    DesertNode node = map.nodes[position.node];
    int next = map.directions.start[position.direction] == 'L' ? node.left : node.right;
    if (next < 0) {
        return { -1, 0 };
    }
    return { next, (position.direction + 1) % map.directions.size };
}


static bool SamePosition(DesertPosition first, DesertPosition second)
{
    return first.node == second.node && first.direction == second.direction;
}


static DesertCycle FindCycle(const DesertMap& map, int node, bool ghosts)
{
    DesertPosition start = { node, 0 };
    DesertPosition slow = NextPosition(map, start);
    DesertPosition fast = NextPosition(map, slow);
    while (!SamePosition(slow, fast)) {
        slow = NextPosition(map, slow);
        fast = NextPosition(map, NextPosition(map, fast));
    }

    DesertCycle cycle = {};
    slow = start;
    while (!SamePosition(slow, fast)) {
        slow = NextPosition(map, slow);
        fast = NextPosition(map, fast);
        ++cycle.prefix;
    }

    fast = NextPosition(map, slow);
    cycle.period = 1;
    while (!SamePosition(slow, fast)) {
        fast = NextPosition(map, fast);
        ++cycle.period;
    }

    int z = NodeDigit('Z');
    int zzz = (z * kAlphabetSize + z) * kAlphabetSize + z;
    DesertPosition position = start;
    for (i64 step = 0; step < cycle.prefix + cycle.period; ++step) {
        if (ghosts ? position.node % kAlphabetSize == z : position.node == zzz) {
            cycle.finishes.push_back(step);
        }
        position = NextPosition(map, position);
    }
    return cycle;
}


static i64 CommonPeriod(i64 first, i64 second)
{
    i64 divisor = first;
    i64 remainder = second;
    while (remainder) {
        i64 next = divisor % remainder;
        divisor = remainder;
        remainder = next;
    }

    i64 result = 0;
    return __builtin_mul_overflow(first / divisor, second, &result) ? 0 : result;
}


static i64 NextFinish(const DesertCycle& cycle, i64 at)
{
    i64 best = -1;
    for (i64 finish : cycle.finishes) {
        if (finish < at) {
            if (finish < cycle.prefix) {
                continue;
            }
            i64 distance = at - finish;
            i64 repeats = distance / cycle.period + (distance % cycle.period != 0);
            if (repeats > (INT64_MAX - finish) / cycle.period) {
                continue;
            }
            finish += repeats * cycle.period;
        }
        if (best < 0 || finish < best) {
            best = finish;
        }
    }
    return best;
}


bool Day08Part1(const File* input_file, i64* answer)
{
    DesertMap map = {};
    if (!ReadDesertMap(input_file, &map) || map.nodes[0].left < 0) {
        return false;
    }
    DesertCycle cycle = FindCycle(map, 0, false);
    if (cycle.finishes.empty()) {
        return false;
    }
    *answer = cycle.finishes[0];
    return true;
}


bool Day08Part2(const File* input_file, i64* answer)
{
    DesertMap map = {};
    if (!ReadDesertMap(input_file, &map)) {
        return false;
    }

    std::vector<DesertCycle> cycles;
    i64 aligned_period = 1;
    bool aligned = true;
    i64 prefix = 0;
    i64 period = 1;
    for (int node = 0; node < kNodeCount; node += kAlphabetSize) {
        if (map.nodes[node].left < 0) {
            continue;
        }
        cycles.push_back(FindCycle(map, node, true));
        const DesertCycle& cycle = cycles.back();
        if (cycle.finishes.empty()) {
            return false;
        }

        i64 first = cycle.finishes[0];
        // The original routes finish at every positive multiple of their first finish.
        if (cycle.period % first || (u64) ((cycle.prefix + cycle.period - 1) / first) != cycle.finishes.size()) {
            aligned = false;
        }
        for (i64 finish : cycle.finishes) {
            aligned = aligned && finish % first == 0;
        }
        if (aligned_period) {
            aligned_period = CommonPeriod(aligned_period, first);
        }
        if (cycle.prefix > prefix) {
            prefix = cycle.prefix;
        }
        if (period) {
            period = CommonPeriod(period, cycle.period);
        }
    }
    if (cycles.empty()) {
        return false;
    }
    if (aligned) {
        *answer = aligned_period;
        return aligned_period != 0;
    }

    i64 limit = period && period - 1 <= INT64_MAX - prefix ? prefix + (period - 1) : INT64_MAX;
    for (i64 step = 1; step <= limit;) {
        i64 next = step;
        for (const DesertCycle& cycle : cycles) {
            i64 finish = NextFinish(cycle, step);
            if (finish < 0) {
                return false;
            }
            if (finish > next) {
                next = finish;
            }
        }
        if (next == step) {
            *answer = step;
            return true;
        }
        step = next;
    }
    return false;
}
