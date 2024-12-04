// day_03_2024.cpp
// AdventOfCode
//
// Copyright (c) 2024 Paul Godavari. All rights reserved. 


#include "util/advent_of_code.h"

#include <vector>


// static const char* input_file_name = "../../2024/input/day_03.test_input";   // Part 1 = 161 
// static const char* input_file_name = "../../2024/input/day_03.test_input2";  // Part 2 = 48
static const char* input_file_name = "../../2024/input/day_03.input";  // Part 1 = 167650499, Part 2 = 95846796


enum class TokenType
{
    Invalid = 0,
    Do,
    Dont,
    Mul
};


struct Token
{
    TokenType type;
    u32 op1;
    u32 op2;
};


bool MatchChar(ParseState* parser, char c)
{
    bool match = false;
    
    if (parser->offset < parser->size && parser->data[parser->offset] == c) {
        match = true;
    }
    
    return match;
}


bool MatchDigit(ParseState* parser)
{
    bool match = false;
    
    if (parser->offset < parser->size) {
        if (parser->data[parser->offset] >= '0' && parser->data[parser->offset] <= '9' ) {
            match = true;
        }
    }
    
    return match;
}


Token MatchDoDont(ParseState* parser)
{
    Token result = {};
    
    if (ConsumeString(parser, { "don't()", 5 })) {
        result.type = TokenType::Dont;
    } else if (ConsumeString(parser, { "do()", 2 })) {
        result.type = TokenType::Do;
    }
    
    return result;
}


Token MatchMul(ParseState* parser)
{
    Token result = {};

    u32 op1 = 0;
    u32 op2 = 0;
    
    if (ConsumeString(parser, { "mul", 3 })) {
        if (MatchChar(parser, '(')) {
            Advance(parser);
            if (MatchDigit(parser)) {
                op1 = ParseNumber(parser);
                if (MatchChar(parser, ',')) {
                    Advance(parser);
                    if (MatchDigit(parser)) {
                        op2 = ParseNumber(parser);
                        if (MatchChar(parser, ')')) {
                            result = { TokenType::Mul, op1, op2 };
                        }
                    }
                }
            }
        }
    }
    
    return result;
}


void Day03_2024()
{
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    u64 part1_sum = 0;
    u64 part2_sum = 0;
    
    ParseState parser = { input_file.data, input_file.size };

    // Part 1
    while (!AtEndOfFile(&parser)) {
        Token token = MatchMul(&parser);
        if (token.type == TokenType::Mul) {
            part1_sum += token.op1 * token.op2;
        } else {
            Advance(&parser);
        }
    }

    f64 part1_stop = MillisecondsSince(run_time_start);
    u64 part2_start = TimeNow();
    
    // Part 2
    parser = { input_file.data, input_file.size, 0 };
    
    std::vector<Token> tokens;
    tokens.reserve(1000);
    
    while (!AtEndOfFile(&parser)) {
        Token token = {};
        
        char current = parser.data[parser.offset];
        switch (current) {
            case 'd':
                token = MatchDoDont(&parser);
                break;
            case 'm':
                token = MatchMul(&parser);
                break;
            default:
                break;
        }
        
        if (token.type == TokenType::Invalid) {
            Advance(&parser);
        } else {
            tokens.push_back(token);
        }
    }
    
    bool enabled = true;
    for (int index = 0; index < tokens.size(); ++index) {
        Token token = tokens[index];
        switch (token.type) {
            case TokenType::Do:
                enabled = true;
                break;
            case TokenType::Dont:
                enabled = false;
                break;
            case TokenType::Mul:
                if (enabled) {
                    part2_sum += token.op1 * token.op2;
                }
                break;                
            default:
                break;
        }
    }
    
    f64 part2_stop = MillisecondsSince(part2_start);

    fprintf(stdout, "2024: Day 03 part 1: %llu (%.4f ms)\n", part1_sum, part1_stop);
    fprintf(stdout, "2024: Day 03 part 2: %llu (%.4f ms)\n", part2_sum, part2_stop);
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));
}

