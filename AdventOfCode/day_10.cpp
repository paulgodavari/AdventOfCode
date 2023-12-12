// day_10.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 
//
// Good explanation for part 2:
// https://www.reddit.com/r/adventofcode/comments/18ey1s7/comment/kcr1jga/


#include "advent_of_code.h"

#include <string.h>


// static const char* input_file_name = "day_10.test_input";  // Part 1: 4, part 2:
// static const char* input_file_name = "day_10.test_input2";  // Part 1: 8
// static const char* input_file_name = "day_10.test_input3";  // Part 2: 4
// static const char* input_file_name = "day_10.test_input4";  // Part 2: 8
static const char* input_file_name = "day_10.input";  // Part 1: 6757, part 2: 523


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
    Position pos = { offset / table.col, offset % table.col };
    return pos;
}


char GetCharAtPosition(ParseState* parser, Position table, Position pos)
{
    char result = -1;
    
    if (pos.col >= 0 && pos.col < (table.col - 1) && pos.row >= 0 && pos.row < table.row) {
        i32 offset = OffsetFromPosition(table, pos);
        assert(offset < parser->size);
        result = parser->data[offset];
    }
    
    return result;
}


void SetCharAtPosition(ParseState* parser, Position table, Position pos, char new_value)
{
    if (pos.col >= 0 && pos.col < (table.col - 1) && pos.row >= 0 && pos.row < table.row) {
        i32 offset = OffsetFromPosition(table, pos);
        assert(offset < parser->size);
        parser->data[offset] = new_value;
    }
}


Move NextDirection(ParseState* parser, Position table, Position position, Move from_direction)
{
    Move result = kMoveInvalid;
    
    char current_char = GetCharAtPosition(parser, table, position);
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
        { 'J', { false,  false,  true,   false,   true } },
    };
    
    if ((next_pos.row >= 0 && next_pos.row < grid.row) && (next_pos.col >= 0 && next_pos.col < grid.col)) {
        char n = GetCharAtPosition(parser, grid, next_pos);
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
    
    MoveInfo first = { kMoveInvalid, kInvalidPosition };
    if (r.dir == kMoveRight) {
        first.pos = r.pos;
        first.dir = kMoveRight;
    } else if (l.dir == kMoveLeft) {
        first.pos = l.pos;
        first.dir = kMoveLeft;
    } else if (u.dir == kMoveUp) {
        first.pos = u.pos;
        first.dir = kMoveUp;
    } else if (d.dir == kMoveDown) {
        first.pos = d.pos;
        first.dir = kMoveRight;
    } else {
        assert(0);
    }
    
    // Find the type of connector at the starting position based on which direction
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

    assert(first.pos != kInvalidPosition);
    assert(first.dir != kMoveInvalid);

    // fprintf(stdout, "Move %s -> (%d, %d)\n", MoveToString(move), next_position.row, next_position.col);
    MoveInfo next_position = first;
    
    u32 steps = 1;
    bool found_loop = false;
    while (!found_loop) {
        char current = GetCharAtPosition(&parser, grid, next_position.pos);
        if (current == 'S') {
            found_loop = true;
            break;
        }
        assert(current != -1);
        next_position.dir = NextDirection(&parser, grid, next_position.pos, next_position.dir);
        next_position.pos = NextPosition(next_position.pos, next_position.dir);
        // fprintf(stdout, "Move %s -> (%d, %d)\n", MoveToString(next_position.dir), next_position.pos.row, next_position.pos.col);
        steps++;
    }
    
    fprintf(stdout, "Day 10 part 1 furthest: %d (steps: %d)\n", steps / 2, steps);
    
    // Part 2.
    
    // Create a copy of the map.
    ParseState map = { new char[input_file.size], input_file.size, 0};
    memset(map.data, '.', input_file.size);

    // Copy the connected path loop.
    found_loop = false;
    next_position = first;
    while (!found_loop) {
        char current = GetCharAtPosition(&parser, grid, next_position.pos);
        if (current == 'S') {
            found_loop = true;
            break;
        }
        SetCharAtPosition(&map, grid, next_position.pos, current);
        assert(current != -1);
        next_position.dir = NextDirection(&parser, grid, next_position.pos, next_position.dir);
        next_position.pos = NextPosition(next_position.pos, next_position.dir);
    }
    
    // Copy the starting character.
    SetCharAtPosition(&map, grid, start_pos, start_char);

    // Print the new map.
//    for (int row = 0; row < grid.row; ++row) {
//        for (int col = 0; col < (grid.col - 1); ++col) {
//            fprintf(stdout, "%c", GetCharAtPosition(&map, grid, { row, col }));
//        }
//        fprintf(stdout, "\n");
//    }
    
    // Raycast left to right to determine which points are inside or outside the path loop.
    u32 inside_count = 0;
    for (int row = 0; row < grid.row; ++row) {
        bool inside = false;
        char last_bend = -1;
        for (int col = 0; col < (grid.col - 1); ++col) {
            char current_char = GetCharAtPosition(&map, grid, { row, col });
            switch (current_char) {
                case '.': {
                    if (inside) {
                        inside_count++;
                    }
                    break;
                }
                case '-': {
                    break;
                }
                case '|': {
                    inside = !inside;
                    break;
                }
                case 'F': {
                    last_bend = 'F';
                    break;
                }
                case '7': {
                    if (last_bend == 'L') {
                        inside = !inside;
                        last_bend = -1;
                    }
                    break;
                }
                case 'J': {
                    if (last_bend == 'F') {
                        inside = !inside;
                        last_bend = -1;
                    }
                    break;
                }
                case 'L': {
                    last_bend = 'L';
                    break;
                }
                default:
                    break;
            }
        }
    }

    fprintf(stdout, "Day 10 part 2: inside count = %u\n", inside_count);
    
    CloseFile(&input_file);
}
