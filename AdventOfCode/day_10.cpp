// day_10.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_10.test_input";  // Part 1: 4, part 2:
// static const char* input_file_name = "day_10.test_input2";  // Part 1: 8
// static const char* input_file_name = "day_10.input";  // Part 1: 6757, part 2:


enum Move
{
    kMoveInvalid = 0,
    kMoveUp,
    kMoveDown,
    kMoveLeft,
    kMoveRight,
    kMoveSize
};


const char* MoveToString(Move move)
{
    switch (move) {
        case kMoveUp: return "up";
        case kMoveDown: return "down";
        case kMoveLeft: return "left";
        case kMoveRight: return "right";
        default: return "invalid";
    }
}


struct Position
{
    i32 row;
    i32 col;
};


struct MoveInfo
{
    Move dir;
    Position pos;
};


struct MoveTable
{
    char symbol;
    bool valid_next[kMoveSize];
};


bool operator==(const Position& lhs, const Position& rhs)
{
    return (lhs.row == rhs.row) && (lhs.col == rhs.col);
}

static const Position kInvalidPosition = { -1, -1 };


i32 OffsetFromPosition(Position table, Position pos)
{
    i32 result = pos.row * table.col + pos.col;
    return result;
}


Position PositionFromOffset(Position table, i32 offset)
{
    Position pos = { offset / table.row, offset % table.col };
    return pos;
}


char CharAtPosition(ParseState* parser, Position table, Position pos)
{
    char result = -1;
    
    if (pos.col >= 0 && pos.col < (table.col - 1) && pos.row >= 0 && pos.row < table.row) {
        i32 offset = OffsetFromPosition(table, pos);
        assert(offset < parser->size);
        result = parser->data[offset];
    }
    
    return result;
}


Move NextDirection(ParseState* parser, Position table, Position position, Move from_direction)
{
    Move result = kMoveInvalid;
    
    char current_char = CharAtPosition(parser, table, position);
    switch (current_char) {
        case '-': {
            assert(from_direction == kMoveRight || from_direction == kMoveLeft);
            result = (from_direction == kMoveRight) ? kMoveRight : kMoveLeft;
            break;
        }
        case '|': {
            assert(from_direction == kMoveUp || from_direction == kMoveDown);
            result = (from_direction == kMoveUp) ? kMoveUp : kMoveDown;
            break;
        }
        case 'F': {
            assert(from_direction == kMoveUp || from_direction == kMoveLeft);
            result = (from_direction == kMoveUp) ? kMoveRight : kMoveDown;
            break;
        }
        case '7': {
            assert(from_direction == kMoveRight || from_direction == kMoveUp);
            result = (from_direction == kMoveRight) ? kMoveDown : kMoveLeft;
            break;
        }
        case 'L': {
            assert(from_direction == kMoveLeft || from_direction == kMoveDown);
            result = (from_direction == kMoveLeft) ? kMoveUp : kMoveRight;
            break;
        }
        case 'J': {
            assert(from_direction == kMoveRight || from_direction == kMoveDown);
            result = (from_direction == kMoveRight) ? kMoveUp : kMoveLeft;
            break;
        }
        default: {
            break;
        }
    }
    
    return result;
}


Position NextPosition(Position current_position, Move move)
{
    Position result = current_position;
    
    switch (move) {
        case kMoveUp: {
            result.row -= 1;
            break;
        }
        case kMoveDown: {
            result.row += 1;
            break;
        }
        case kMoveLeft: {
            result.col -= 1;
            break;
        }
        case kMoveRight: {
            result.col += 1;
            break;
        }
        default: {
            result = kInvalidPosition;
            break;
        }
    }
    
    return result;
}


MoveInfo CanMove(ParseState* parser, Position grid, Move direction, Position current_position)
{
    MoveInfo result = { kMoveInvalid, kInvalidPosition };
    
    Position next_pos = current_position;
    switch (direction) {
        case kMoveUp:
            next_pos.row = current_position.row - 1;
            break;
        case kMoveDown:
            next_pos.row = current_position.row + 1;
            break;
        case kMoveLeft:
            next_pos.col = current_position.col - 1;
            break;
        case kMoveRight:
            next_pos.col = current_position.col + 1;
            break;
        default:
            break;
    }

    MoveTable valid_moves_table[] = {
        //     invalid,     up,   down,   left,  right
        { '-', { false,  false,  false,   true,   true } },
        { '|', { false,   true,   true,  false,  false } },
        { 'F', { false,  false,   true,   true,  false } },
        { '7', { false,   true,  false,  false,   true } },
        { 'L', { false,  false,   true,   true,  false } },
        { 'J', { false,  false,  true,  false,    true } },
    };
    
    if ((next_pos.row >= 0 && next_pos.row < grid.row) && (next_pos.col >= 0 && next_pos.col < grid.col)) {
        char n = CharAtPosition(parser, grid, next_pos);
        bool valid_move = false;
        
        for (int i = 0; i < 6; ++i) {
            if (n == valid_moves_table[i].symbol) {
                valid_move = valid_moves_table[i].valid_next[direction];
                break;
            }
        }
        
        if (valid_move) {
            result.dir = direction;
            result.pos = next_pos;
        }
    }
    
    return result;
}


void Day10()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stdout, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    // Find the line length so we can move up and down by row.
    i32 columns = 0;
    while (!AtEndOfLine(&parser)) {
        columns++;
        Advance(&parser);
    }
    columns++;  // Account for the '\n' character in the last column.
    fprintf(stdout, "Column count: %u\n", columns);
    i32 rows = (i32) parser.size / columns;
    fprintf(stdout, "Row count: %u\n", rows);
    
    Position grid = { rows, columns };
    
    // Reset the parser.
    parser.offset = 0;
    
    // Find the start position.
    i32 start_offset = -1;
    while (!AtEndOfFile(&parser)) {
        if (parser.data[parser.offset] == 'S') {
            start_offset = parser.offset;
            break;
        }
        Advance(&parser);
    }

    Position start_pos = PositionFromOffset(grid, start_offset);
    fprintf(stderr, "Starting position %d (%d, %d)\n", start_offset, start_pos.row, start_pos.col);
    if (start_offset < 0) {
        return;
    }
    
    parser.offset = start_offset;
    
    // Find the first pipe that connects to the starting position.
    MoveInfo r = CanMove(&parser, grid, kMoveRight, start_pos);
    MoveInfo l = CanMove(&parser, grid, kMoveLeft, start_pos);
    MoveInfo u = CanMove(&parser, grid, kMoveUp, start_pos);
    MoveInfo d = CanMove(&parser, grid, kMoveDown, start_pos);
    
    Position next_position = kInvalidPosition;
    Move move = kMoveInvalid;
    if (r.dir == kMoveRight) {
        next_position = r.pos;
        move = kMoveRight;
    } else if (l.dir == kMoveLeft) {
        next_position = l.pos;
        move = kMoveLeft;
    } else if (u.dir == kMoveUp) {
        next_position = u.pos;
        move = kMoveUp;
    } else if (d.dir == kMoveDown) {
        next_position = d.pos;
        move = kMoveRight;
    } else {
        assert(0);
    }
    
    // Find the type of connector that would be at the starting position based on which direction
    // we can move in.
    char start_char = '?';
    if (r.dir == kMoveRight) {
        if (l.dir == kMoveLeft) {
            start_char = '-';
        } else if (u.dir == kMoveUp) {
            start_char = 'L';
        } else if (d.dir == kMoveDown) {
            start_char = 'F';
        } else {
            assert(0);
        }
    }
    if (l.dir == kMoveLeft) {
        if (u.dir == kMoveUp) {
            start_char = 'J';
        } else if (d.dir == kMoveDown) {
            start_char = '7';
        } else {
            assert(0);
        }
    }
    if (u.dir == kMoveUp) {
        if (d.dir == kMoveDown) {
            start_char = '|';
        }
    }
    
    fprintf(stdout, "Start character: '%c'\n", start_char);

    assert(next_position != kInvalidPosition);
    assert(move != kMoveInvalid);

    // fprintf(stdout, "Move %s -> (%d, %d)\n", MoveToString(move), next_position.row, next_position.col);
    
    u32 steps = 1;
    bool found_loop = false;
    while (!found_loop) {
        char current = CharAtPosition(&parser, grid, next_position);
        if (current == 'S') {
            found_loop = true;
            break;
        }
        assert(current != -1);
        move = NextDirection(&parser, grid, next_position, move);
        next_position = NextPosition(next_position, move);
        // fprintf(stdout, "Move %s -> (%d, %d)\n", MoveToString(move), next_position.row, next_position.col);
        steps++;
    }
    
    fprintf(stdout, "Day 10 part 1 furthest: %d (steps: %d)\n", steps / 2, steps);
    
    CloseFile(&input_file);
}
