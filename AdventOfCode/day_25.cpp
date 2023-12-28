// day_25.cpp
// AdventOfCode
//
// Copyright (c) 2023 Paul Godavari. All rights reserved. 


#include "advent_of_code.h"

#include <unordered_map>
#include <vector>

static const char* input_file_name = "day_25.test_input";  // Part 1: , part 2: 
// static const char* input_file_name = "day_25.input";  // Part 1: , part 2: 


static const u32 kMaxEdges = 8;


struct EdgeList
{
    u32 count;
    String edges[kMaxEdges];
};


struct Node
{
    i32 index;
    String name;
    EdgeList edges;
};


void Day25()
{
    fprintf(stdout, "Day 25\n");
    
    u64 run_time = TimeNow();
    
    File input_file = ReadFile(input_file_name);
    if (!input_file.handle) {
        fprintf(stderr, "Error reading: %s\n", input_file_name);
        return;
    }
    
    ParseState parser = { input_file.data, input_file.size, 0 };
    
    std::vector<Node*> nodes;
    std::unordered_map<String, Node*, StringHash> node_map;
    
    while (!AtEndOfFile(&parser)) {
        String node_name = ParseWord(&parser);
        assert(parser.data[parser.offset] == ':');
        Advance(&parser, 2);
        Node* node = new Node { (i32) nodes.size(), node_name, {} };
        while (!AtEndOfLine(&parser)) {
            String edge = ParseWord(&parser);
            node->edges.edges[node->edges.count] = edge;
            node->edges.count++;
        }
        Advance(&parser);
        
        nodes.push_back(node);
        node_map[node->name] = node;
    }
        
    fprintf(stdout, "Initialization time: %.4f ms\n", MillisecondsSince(run_time));
    
    for (int i = 0; i < nodes.size(); ++i) {
        Node* node = nodes[i];
        fprintf(stdout, "Node %u: %.*s: ", node->index, (int) node->name.size, node->name.start);
        for (int i = 0; i != node->edges.count; ++i) {
            fprintf(stdout, "%.*s -- ", (int) node->edges.edges[i].size, node->edges.edges[i].start);
        }
        fprintf(stdout, "\n");
    }
    
    CloseFile(&input_file);
}
