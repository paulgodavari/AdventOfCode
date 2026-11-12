// day_16.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <vector>


enum BeamDirection
{
    kBeamUp,
    kBeamRight,
    kBeamDown,
    kBeamLeft,
};


struct Beam
{
    int row;
    int col;
    BeamDirection direction;
};


static Beam Step(Beam beam)
{
    const int row_delta[] = { -1, 0, 1, 0 };
    const int col_delta[] = { 0, 1, 0, -1 };
    beam.row += row_delta[beam.direction];
    beam.col += col_delta[beam.direction];
    return beam;
}


static i64 TraceLight(const std::vector<String>& rows, Beam start)
{
    int height = (int) rows.size();
    int width = (int) rows[0].size;
    std::vector<u8> visited((size_t) height * width);
    std::vector<Beam> pending = { start };
    i64 energized = 0;
    while (!pending.empty()) {
        Beam beam = pending.back();
        pending.pop_back();
        while (beam.row >= 0 && beam.row < height && beam.col >= 0 && beam.col < width) {
            u8& cell = visited[(size_t) beam.row * width + beam.col];
            u8 direction = 1 << beam.direction;
            if (cell & direction) {
                break;
            }
            energized += cell == 0;
            cell |= direction;

            switch (rows[beam.row].start[beam.col]) {
                case '/':
                    beam.direction = (BeamDirection) (beam.direction ^ 1);
                    break;
                case '\\':
                    beam.direction = (BeamDirection) (beam.direction ^ 3);
                    break;
                case '-':
                    if (beam.direction == kBeamUp || beam.direction == kBeamDown) {
                        pending.push_back(Step({ beam.row, beam.col, kBeamLeft }));
                        beam.direction = kBeamRight;
                    }
                    break;
                case '|':
                    if (beam.direction == kBeamLeft || beam.direction == kBeamRight) {
                        pending.push_back(Step({ beam.row, beam.col, kBeamUp }));
                        beam.direction = kBeamDown;
                    }
                    break;
            }

            beam = Step(beam);
        }
    }
    return energized;
}


static bool SolveDay16(const File* input_file, bool all_edges, i64* answer)
{
    String input = { input_file->data, input_file->size };
    std::vector<String> rows;
    while (input.size) {
        String row = ReadLine(&input);
        if (!row.size || row.size > INT32_MAX || rows.size() >= INT32_MAX || (!rows.empty() && row.size != rows[0].size)) {
            return false;
        }
        for (size_t col = 0; col < row.size; ++col) {
            char cell = row.start[col];
            if (cell != '.' && cell != '/' && cell != '\\' && cell != '-' && cell != '|') {
                return false;
            }
        }
        rows.push_back(row);
    }
    if (rows.empty()) {
        return false;
    }

    if (!all_edges) {
        *answer = TraceLight(rows, { 0, 0, kBeamRight });
        return true;
    }

    i64 best = 0;
    int height = (int) rows.size();
    int width = (int) rows[0].size;
    for (int col = 0; col < width; ++col) {
        i64 down = TraceLight(rows, { 0, col, kBeamDown });
        i64 up = TraceLight(rows, { height - 1, col, kBeamUp });
        if (down > best) best = down;
        if (up > best) best = up;
    }
    for (int row = 0; row < height; ++row) {
        i64 right = TraceLight(rows, { row, 0, kBeamRight });
        i64 left = TraceLight(rows, { row, width - 1, kBeamLeft });
        if (right > best) best = right;
        if (left > best) best = left;
    }
    *answer = best;
    return true;
}


bool Day16Part1(const File* input_file, i64* answer)
{
    return SolveDay16(input_file, false, answer);
}


bool Day16Part2(const File* input_file, i64* answer)
{
    return SolveDay16(input_file, true, answer);
}
