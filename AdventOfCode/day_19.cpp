// day_19.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <string>
#include <unordered_map>
#include <vector>


static const char* input_file_name = "day_19.test_input";  // Part 1: 19114, part 2:
// static const char* input_file_name = "day_19.input";  // Part 1: 425811, part 2:

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
    kAttributeSize
};


struct Component
{
    union {
        struct {
            u32 invalid;
            u32 x;
            u32 m;
            u32 a;
            u32 s;
        };
        u32 c[kAttributeSize];
    };
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
    
    // std::vector<RuleList> list;
    std::unordered_map<std::string, RuleList> all_rules;
    
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
        
        // list.push_back(rule_list);
        std::string rule_name(rule_list.name.start, rule_list.name.size);
        all_rules[rule_name] = rule_list;
    }
    
    // Parse the components.
    std::vector<Component> components;
    while (!AtEndOfFile(&parser)) {
        Advance(&parser);  // '{'
        assert(parser.data[parser.offset] == 'x');
        Advance(&parser, 2);
        u32 x = ParseNumber(&parser);

        Advance(&parser);  // ','
        assert(parser.data[parser.offset] == 'm');
        Advance(&parser, 2);
        u32 m = ParseNumber(&parser);

        Advance(&parser);  // ','
        assert(parser.data[parser.offset] == 'a');
        Advance(&parser, 2);
        u32 a = ParseNumber(&parser);

        Advance(&parser);  // ','
        assert(parser.data[parser.offset] == 's');
        Advance(&parser, 2);
        u32 s = ParseNumber(&parser);

        assert(parser.data[parser.offset] == '}');
        Advance(&parser, 2);  // '}' + '\n'
        
        Component c = { 0, x, m, a, s };
        components.push_back(c);
    }
    
    u64 part_1_sum = 0;
    for (int i = 0; i < components.size(); ++i) {
        Component c = components[i];
        fprintf(stdout, "{x=%u,m=%u,a=%u,s=%u}: ", c.x, c.m, c.a, c.s);

        std::string rule_name = "in";

        bool done_component = false;
        bool accepted = false;
        while (!done_component) {
            
           RuleList rule_list = all_rules[rule_name];
            fprintf(stdout, " -> %.*s", (int) rule_list.name.size, rule_list.name.start);
            for (int r = 0; r < rule_list.count; ++r) {
                bool done_rule_list = false;
                Rule rule = rule_list.rules[r];
                switch (rule.op) {
                    case kOpLess: {
                        if (c.c[rule.attr] < rule.value) {
                            // Go to rule.next_rule
                            done_rule_list = true;
                            rule_name = std::string(rule.next_rule.start, rule.next_rule.size);
                        }
                        break;
                    }
                    case kOpGreater: {
                        if (c.c[rule.attr] > rule.value) {
                            // Go to rule.next_rule
                            done_rule_list = true;
                            rule_name = std::string(rule.next_rule.start, rule.next_rule.size);
                        }
                        break;
                    }
                    case kOpNext: {
                        done_rule_list = true;
                        rule_name = std::string(rule.next_rule.start, rule.next_rule.size);
                        break;
                    }
                    case kOpReject: {
                        done_component = true;
                        done_rule_list = true;
                        break;
                    }
                    case kOpAccept: {
                        done_component = true;
                        done_rule_list = true;
                        accepted = true;
                        break;
                    }
                    default: {
                        break;
                    }
                }
                if (done_rule_list) {
                    break;
                }
            }
            
            if (rule_name == "A") {
                done_component = true;
                accepted = true;
            } else if (rule_name == "R") {
                done_component = true;
            }
            
             if (done_component) {
                break;
            }
        }
        
        if (accepted) {
            part_1_sum += c.x + c.m + c.a + c.s;
        }
        fprintf(stdout, " -> %c\n", accepted ? 'A' : 'R');
    }
    
    fprintf(stdout, "Part 1 sum: %llu\n", part_1_sum);
    
    CloseFile(&input_file);
}


