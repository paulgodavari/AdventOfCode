// day_16.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"


static const char* input_file_name = "day_16.test_input";  // Part 1: 46, part2: 51
// static const char* input_file_name = "day_16.input";  // Part 1: 7472, part2: 7716

static const u32 kMaxRows = 110;
static const u32 kMaxColumns = 110;


enum Direction
{
    kDirectionInvalid = 0,
    kDirectionUp,
    kDirectionDown,
    kDirectionLeft,
    kDirectionRight,
    kDirectionSize
};


struct Position
{
    i32 row;
    i32 col;
};


struct Move
{
    Direction dir;
    Position pos;
};


struct GridItem
{
    // First position is used for having been visited at all.
    bool visited[kDirectionSize];
};


struct Grid
{
    u32 rows;
    u32 cols;
    GridItem items[kMaxRows][kMaxColumns];
};


bool ValidPosition(Grid* grid, Position position)
{
    bool result = ((position.row >= 0) && (position.row < grid->rows) &&
                   (position.col >= 0) && (position.col < (grid->cols - 1)));
    return result;
}


void TraceLightRay(ParseState* parser, Grid* visited_grid, Move move)
{
    while (ValidPosition(visited_grid, move.pos)) {
        if (visited_grid->items[move.pos.row][move.pos.col].visited[move.dir]) {
            // Already visited from this direction, we can skip the rest of the path.
            return;
        }
        
        // Mark as visited:
        visited_grid->items[move.pos.row][move.pos.col].visited[0] = true;
        visited_grid->items[move.pos.row][move.pos.col].visited[move.dir] = true;
        
        u32 offset = move.pos.row * visited_grid->cols + move.pos.col;
        char terrain = parser->data[offset];
        switch (terrain) {
            case '.': {
                switch (move.dir) {
                    case kDirectionUp:
                        move.pos.row--;
                        break;
                    case kDirectionDown:
                        move.pos.row++;
                        break;
                    case kDirectionLeft:
                        move.pos.col--;
                        break;
                    case kDirectionRight:
                        move.pos.col++;
                        break;
                    default:
                        assert(0);
                        break;
                }
                break;
            }
            case '\\': {
                switch (move.dir) {
                    case kDirectionUp:
                        move.pos.col--;
                        move.dir = kDirectionLeft;                        
                        break;
                    case kDirectionDown:
                        move.pos.col++;
                        move.dir = kDirectionRight;                        
                        break;
                    case kDirectionLeft:
                        move.pos.row--;
                        move.dir = kDirectionUp;                        
                        break;
                    case kDirectionRight:
                        move.pos.row++;
                        move.dir = kDirectionDown;
                        break;
                    default:
                        assert(0);
                        break;
                }
                break;
            }
            case '/': {
                switch (move.dir) {
                    case kDirectionUp:
                        move.pos.col++;
                        move.dir = kDirectionRight;                        
                        break;
                    case kDirectionDown:
                        move.pos.col--;
                        move.dir = kDirectionLeft;                        
                        break;
                    case kDirectionLeft:
                        move.pos.row++;
                        move.dir = kDirectionDown;                        
                        break;
                    case kDirectionRight:
                        move.pos.row--;
                        move.dir = kDirectionUp;
                        break;
                    default:
                        assert(0);
                        break;
                }
                break;
            }
            case '-': {
                switch (move.dir) {
                    case kDirectionUp:
                    case kDirectionDown: {
                        Move left = { kDirectionLeft, { move.pos.row, move.pos.col - 1 } };
                        TraceLightRay(parser, visited_grid, left);
                        Move right = { kDirectionRight, { move.pos.row, move.pos.col + 1 } };
                        TraceLightRay(parser, visited_grid, right);
                        break;
                    }
                    case kDirectionLeft:
                        move.pos.col--;
                        break;
                    case kDirectionRight:
                        move.pos.col++;
                        break;
                    default:
                        assert(0);
                        break;
                }
                break;
            }
            case '|': {
                switch (move.dir) {
                    case kDirectionUp:
                        move.pos.row--;
                        break;
                    case kDirectionDown:
                        move.pos.row++;
                        break;
                    case kDirectionLeft:
                    case kDirectionRight: {
                        Move up = { kDirectionUp, { move.pos.row - 1, move.pos.col } };
                        TraceLightRay(parser, visited_grid, up);
                        Move down = { kDirectionDown, { move.pos.row + 1, move.pos.col } };
                        TraceLightRay(parser, visited_grid, down);
                        break;
                    }
                    default:
                        assert(0);
                        break;
                }
                break;
            }
            default: {
                assert(0);
                break;
            }
        }        
    }
}


u32 ScoreLightRayTrace(ParseState* parser, Grid* grid, Move starting_move)
{
    u32 result = 0;
    
    TraceLightRay(parser, grid, starting_move);
    
    for (int row = 0; row < grid->rows; ++row) {
        for (int col = 0; col < (grid->cols - 1); ++col) {
            if (grid->items[row][col].visited[0]) {
                result++;
            }
        }
    }
    
    return result;
}


void Day16()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    u32 column_count = 0;
    while (!AtEndOfLine(&parser)) {
        column_count++;
        Advance(&parser);
    }
    column_count++;  // '\n' character
    u32 row_count = (int) input_file.size / column_count;
    fprintf(stdout, "rows: %u, cols: %u\n", row_count, column_count);
    
    // Reset
    parser.offset = 0;
    
    // Part 1 ----------------------------
    
    Grid visited = { row_count, column_count };
    Move inital_pos = { kDirectionRight };
    
    u32 energized = ScoreLightRayTrace(&parser, &visited, inital_pos);
    fprintf(stdout, "Day 16 part 1 energized: %u\n", energized);
    
    // Part 2 ----------------------------
    
    energized = 0;
    
    // Check all the column possibilities.
    for (int col = 0; col < (visited.cols - 1); ++col) {
        inital_pos = { kDirectionDown, { 0, col } };
        visited = {  row_count, column_count };
        u32 enery_col = ScoreLightRayTrace(&parser, &visited, inital_pos);
        if (enery_col > energized) {
            energized = enery_col;
        }
        
        inital_pos = { kDirectionUp, { (i32) visited.rows - 1, col } };
        visited = { row_count, column_count };
        enery_col = ScoreLightRayTrace(&parser, &visited, inital_pos);
        if (enery_col > energized) {
            energized = enery_col;
        }
    }

    // Check all the row possibilities.
    for (int row = 0; row < visited.rows; ++row) {
        inital_pos = { kDirectionRight, { row, 0 } };
        visited = { row_count, column_count };
        u32 enery_row = ScoreLightRayTrace(&parser, &visited, inital_pos);
        if (enery_row > energized) {
            energized = enery_row;
        }
        
        inital_pos = { kDirectionLeft, { row, (i32) visited.cols - 2 } };
        visited = { row_count, column_count };
        enery_row = ScoreLightRayTrace(&parser, &visited, inital_pos);
        if (enery_row > energized) {
            energized = enery_row;
        }
    }
    
    fprintf(stdout, "Day 16 part 2 energized: %u\n", energized);
    
    CloseFile(&input_file);
}


