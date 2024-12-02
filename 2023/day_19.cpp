// day_19.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <string>
#include <unordered_map>
#include <vector>


// static const char* input_file_name = "day_19.test_input";  // Part 1: 19114, part 2: 167409079868000
static const char* input_file_name = "day_19.input";  // Part 1: 425811, part 2: 131796824371749

static const u32 kMaxRules = 10;
static const u32 kMinAttrValue = 1;
static const u32 kMaxAttrValue = 4000;


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

typedef std::unordered_map<std::string, RuleList> RuleMap;


struct AttributeValue
{
    Attribute attr;
    u32 min;
    u32 max;
};


struct AttributeList
{
    AttributeValue value[kAttributeSize];
};


struct Node
{
    std::string rule_name;
    u32 rule_index;
    AttributeList attrs;
    Node* left;
    AttributeValue left_attr;
    Node* right;
    AttributeValue right_attr;
};


u64 ComputeAttributeSum(AttributeList attrs)
{
    u64 result = 1;
    for (int i = 1; i < kAttributeSize; ++i) {
        result *= attrs.value[i].max - attrs.value[i].min + 1;
    }
    return result;
}


u64 ComputeAcceptedRulesSum(AttributeList parent_attrs, std::string rule_name, u32 rule_index, RuleMap* rule_map)
{
    if (rule_name == "A") {
        return ComputeAttributeSum(parent_attrs);
    } else if (rule_name == "R") {
        return 0;
    }
    
    RuleList rules = (*rule_map)[rule_name];
    Rule rule = rules.rules[rule_index];
    
    while (rule.op == kOpNext) {
        rule_name = std::string(rule.next_rule.start, rule.next_rule.size);
        rule_index = 0;
        rules = (*rule_map)[rule_name];
        rule = rules.rules[rule_index];
    }
    
    if (rule.op == kOpAccept) {
        return ComputeAttributeSum(parent_attrs);
    } else if (rule.op == kOpReject) {
        return 0;
    }
    
    // Create the branch ranges. Inherit from the parent values, and update
    // based on the rule attribute (left <- lower, right <- higher).
    AttributeValue low = parent_attrs.value[rule.attr];
    AttributeValue high = parent_attrs.value[rule.attr];
    
    std::string left_rule_name = rule_name;
    u32 left_rule_index = rule_index + 1;
    
    std::string right_rule_name = rule_name;
    u32 right_rule_index = rule_index + 1;

    if (rule.op == kOpLess) {
        low.max = rule.value - 1;
        high.min = rule.value;
        left_rule_name = std::string(rule.next_rule.start, rule.next_rule.size);
        left_rule_index = 0;
    } else if (rule.op == kOpGreater) {
        low.max = rule.value;
        high.min = rule.value + 1;
        right_rule_name = std::string(rule.next_rule.start, rule.next_rule.size);
        right_rule_index = 0;
    }
    
    AttributeList left_attr = parent_attrs;
    left_attr.value[rule.attr] = low;
    u64 left_sum = ComputeAcceptedRulesSum(left_attr, left_rule_name, left_rule_index, rule_map);

    AttributeList right_attr = parent_attrs;
    right_attr.value[rule.attr] = high;
    u64 right_sum = ComputeAcceptedRulesSum(right_attr, right_rule_name, right_rule_index, rule_map);
    
    return left_sum + right_sum;
}


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
    u64 run_time_start = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    RuleMap all_rules;
    
    fprintf(stdout, "Day 19\nInitialization time: %.4f ms\n", MillisecondsSince(run_time_start));
    u64 start_time = TimeNow();
    
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
        
        std::string rule_name(rule_list.name.start, rule_list.name.size);
        all_rules[rule_name] = rule_list;
    }
    
    fprintf(stdout, "Rules parse time: %.4f ms\n", MillisecondsSince(start_time));
    start_time = TimeNow();
    
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
    
    fprintf(stdout, "Components parse time: %.4f ms\n", MillisecondsSince(start_time));
    start_time = TimeNow();

    u64 part_1_sum = 0;
    for (int i = 0; i < components.size(); ++i) {
        Component c = components[i];
        // fprintf(stdout, "{x=%u,m=%u,a=%u,s=%u}: ", c.x, c.m, c.a, c.s);

        std::string rule_name = "in";

        bool done_component = false;
        bool accepted = false;
        while (!done_component) {
            
           RuleList rule_list = all_rules[rule_name];
            // fprintf(stdout, " -> %.*s", (int) rule_list.name.size, rule_list.name.start);
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
        // fprintf(stdout, " -> %c\n", accepted ? 'A' : 'R');
    }
    
    fprintf(stdout, "Part 1 sum: %llu (%.4f ms)\n", part_1_sum, MillisecondsSince(start_time));
    
    
    // Part 2
    start_time = TimeNow();
    
    AttributeList initial_attrs = {};
    for (u32 i = kAttributeInvalid; i < kAttributeSize; ++i) {
        initial_attrs.value[i] = { (Attribute) i, kMinAttrValue, kMaxAttrValue };
    }
    u64 part_2_sum = ComputeAcceptedRulesSum(initial_attrs, "in", 0, &all_rules);

    fprintf(stdout, "Part 2 sum: %llu (%.4f ms)\n", part_2_sum, MillisecondsSince(start_time));
    
    fprintf(stdout, "Total time: %.4f ms\n", MillisecondsSince(run_time_start));

    CloseFile(&input_file);
}


