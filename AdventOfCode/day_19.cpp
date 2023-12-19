// day_19.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <vector>


static const char* input_file_name = "day_19.test_input";  // Part 1: , part 2:
// static const char* input_file_name = "day_19.input";  // Part 1: , part 2:

static const u32 kMaxRules = 10;


enum Operation
{
    kOpInvalid = 0,
    kOpLess,
    kOpGreater,
    kOpNext,
    kOpAccept,
    kOpReject
};


enum Attribute
{
    kAttributeInvalid = 0,
    kAttributeX,
    kAttributeM,
    kAttributeA,
    kAttributeS,
};


struct Rule
{
    Attribute attr;
    Operation op;
    u32 value;
    String next_rule;
};


struct RuleList
{
    String name;
    u32 count;
    Rule rules[kMaxRules];
};


Attribute AttributeFromString(String attr_str)
{
    Attribute result = kAttributeInvalid;
    switch (attr_str.start[0]) {
        case 'x':
            result = kAttributeX;
            break;
        case 'm':
            result = kAttributeM;
            break;
        case 'a':
            result = kAttributeA;
            break;
        case 's':
            result = kAttributeS;
            break;
    }
    return result;
}


void Day19()
{
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    fprintf(stdout, "Day 19\n");
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    std::vector<RuleList> list;
    
    // Parse the rules.
    bool done_parsing_rules = false;
    while (!done_parsing_rules) {
        // Parse the rule name.
        RuleList rule_list = {};
        rule_list.name = ParseWord(&parser);
        assert(parser.data[parser.offset] == '{');
        Advance(&parser);  // Skip the '{'
        
        // Parse a variable length list of rules.
        bool done_parsing_rule_list = false;
        while (!done_parsing_rule_list) {
            Rule r = {};
            String word = ParseWord(&parser);
            char current_char = parser.data[parser.offset];
            switch (current_char) {
                case '<':
                case '>': {
                    Operation op = (current_char == '>') ? kOpGreater : kOpLess;
                    Advance(&parser);  // Move past the operator
                    u32 value = ParseNumber(&parser);
                    Advance(&parser);  // Move past the ':'
                    String next_rule = ParseWord(&parser);
                    r = { AttributeFromString(word), op, value, next_rule };
                    break;
                }
                default: {
                    if (word == CONST_STRING("A")) {
                        r.op = kOpAccept;
                    } else if (word == CONST_STRING("R")) {
                        r.op = kOpReject;
                    } else {
                        r.op = kOpNext;
                        r.next_rule = word;
                    }
                    break;
                }
            }

            rule_list.rules[rule_list.count] = r;
            rule_list.count++;

            if (parser.data[parser.offset] == ',') {
                Advance(&parser);
            }
            if (parser.data[parser.offset] == '}') {
                done_parsing_rule_list = true;
                Advance(&parser);
            }
        }
        
        Advance(&parser);  // Move to the next line.
        if (parser.data[parser.offset] == '\n') {
            done_parsing_rules = true;
            Advance(&parser);
        }
        
        list.push_back(rule_list);
    }
    
    CloseFile(&input_file);
}


