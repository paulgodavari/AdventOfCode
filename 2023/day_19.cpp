// day_19.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved.


#include "util/days.h"

#include <string.h>
#include <unordered_map>
#include <vector>


struct WorkflowRule
{
    String destination;
    int field;
    char operation;
    i64 limit;
};


struct RatingRange
{
    i64 first;
    i64 end;
};


struct Ratings
{
    RatingRange fields[4];
};


using Workflows = std::unordered_map<String, std::vector<WorkflowRule>, StringHash>;


static int FieldIndex(char field)
{
    switch (field) {
        case 'x': return 0;
        case 'm': return 1;
        case 'a': return 2;
        case 's': return 3;
    }
    return -1;
}


static bool ReadWorkflows(String* input, Workflows* workflows)
{
    while (input->size) {
        String line = ReadLine(input);
        if (!line.size) {
            break;
        }

        const char* brace = (const char*) memchr(line.start, '{', line.size);
        if (!brace || brace == line.start || line.start[line.size - 1] != '}') {
            return false;
        }
        String name = { line.start, (size_t) (brace - line.start) };
        if (workflows->find(name) != workflows->end()) {
            return false;
        }
        auto& rules = (*workflows)[name];
        String remaining = { brace + 1, line.size - name.size - 2 };
        while (remaining.size) {
            const char* comma = (const char*) memchr(remaining.start, ',', remaining.size);
            size_t size = comma ? (size_t) (comma - remaining.start) : remaining.size;
            String token = { remaining.start, size };
            const char* colon = (const char*) memchr(token.start, ':', token.size);
            if (colon) {
                if (colon - token.start < 3 || colon == token.start + token.size - 1) {
                    return false;
                }
                int field = FieldIndex(token.start[0]);
                char operation = token.start[1];
                String value = { token.start + 2, (size_t) (colon - token.start - 2) };
                i64 limit = 0;
                if (field < 0 || (operation != '<' && operation != '>') || !ReadInteger(&value, &limit) || value.size) {
                    return false;
                }
                rules.push_back({ { colon + 1, (size_t) (token.start + token.size - colon - 1) }, field, operation, limit });
            } else {
                if (!token.size || comma) {
                    return false;
                }
                rules.push_back({ token, -1 });
            }

            size += comma != nullptr;
            remaining.start += size;
            remaining.size -= size;
        }
        if (rules.empty() || rules.back().field >= 0) {
            return false;
        }
    }
    return workflows->find(CONST_STRING("in")) != workflows->end();
}


static bool ReadComponent(String line, i64* values)
{
    if (line.size < 2 || *line.start != '{' || line.start[line.size - 1] != '}') {
        return false;
    }
    ++line.start;
    line.size -= 2;

    bool seen[4] = {};
    for (int index = 0; index < 4; ++index) {
        if (line.size < 3 || line.start[1] != '=') {
            return false;
        }
        int field = FieldIndex(*line.start);
        if (field < 0 || seen[field]) {
            return false;
        }
        seen[field] = true;
        line.start += 2;
        line.size -= 2;
        if (!ReadInteger(&line, &values[field]) || values[field] <= 0) {
            return false;
        }

        if (index < 3) {
            if (!line.size || *line.start != ',') {
                return false;
            }
            ++line.start;
            --line.size;
        }
    }
    return line.size == 0;
}


static bool AcceptComponent(const Workflows& workflows, const i64* values, bool* accepted)
{
    String name = CONST_STRING("in");
    for (size_t depth = 0; depth <= workflows.size(); ++depth) {
        if (name == CONST_STRING("A") || name == CONST_STRING("R")) {
            *accepted = name == CONST_STRING("A");
            return true;
        }
        auto workflow = workflows.find(name);
        if (workflow == workflows.end()) {
            return false;
        }
        for (const WorkflowRule& rule : workflow->second) {
            if (rule.field < 0 || (rule.operation == '<' && values[rule.field] < rule.limit) ||
                (rule.operation == '>' && values[rule.field] > rule.limit)) {
                name = rule.destination;
                break;
            }
        }
    }
    return false;
}


static bool CountAccepted(const Workflows& workflows, String name, Ratings ratings, size_t depth, i64* total)
{
    i64 combinations = 1;
    for (RatingRange range : ratings.fields) {
        if (range.first >= range.end) {
            return true;
        }
        combinations *= range.end - range.first;
    }

    if (name == CONST_STRING("A")) {
        *total += combinations;
        return true;
    }
    if (name == CONST_STRING("R")) {
        return true;
    }
    auto workflow = workflows.find(name);
    if (workflow == workflows.end() || depth >= workflows.size()) {
        return false;
    }

    for (const WorkflowRule& rule : workflow->second) {
        if (rule.field < 0) {
            return CountAccepted(workflows, rule.destination, ratings, depth + 1, total);
        }

        Ratings matched = ratings;
        RatingRange& yes = matched.fields[rule.field];
        RatingRange& no = ratings.fields[rule.field];
        if (rule.operation == '<') {
            if (rule.limit < yes.end) yes.end = rule.limit;
            if (rule.limit > no.first) no.first = rule.limit;
        } else {
            i64 boundary = rule.limit == INT64_MAX ? INT64_MAX : rule.limit + 1;
            if (boundary > yes.first) yes.first = boundary;
            if (boundary < no.end) no.end = boundary;
        }

        if (!CountAccepted(workflows, rule.destination, matched, depth + 1, total)) {
            return false;
        }
        if (no.first >= no.end) {
            return true;
        }
    }
    return false;
}


bool Day19Part1(const File* input_file, i64* answer)
{
    String input = { input_file->data, input_file->size };
    Workflows workflows;
    if (!ReadWorkflows(&input, &workflows)) {
        return false;
    }

    i64 total = 0;
    while (input.size) {
        i64 values[4] = {};
        bool accepted = false;
        if (!ReadComponent(ReadLine(&input), values) || !AcceptComponent(workflows, values, &accepted)) {
            return false;
        }
        if (accepted) {
            for (i64 value : values) {
                if (__builtin_add_overflow(total, value, &total)) {
                    return false;
                }
            }
        }
    }
    *answer = total;
    return true;
}


bool Day19Part2(const File* input_file, i64* answer)
{
    String input = { input_file->data, input_file->size };
    Workflows workflows;
    if (!ReadWorkflows(&input, &workflows)) {
        return false;
    }

    Ratings ratings = { { { 1, 4001 }, { 1, 4001 }, { 1, 4001 }, { 1, 4001 } } };
    *answer = 0;
    return CountAccepted(workflows, CONST_STRING("in"), ratings, 0, answer);
}
