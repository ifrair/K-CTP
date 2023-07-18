#pragma once
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>

#include "../Utiles/Logger.h"

using Edges = std::vector<std::unordered_map<std::size_t, double>>;

struct Graph {
    std::size_t num_vertices;
    std::size_t num_edges;
    Edges edges;
};


// loading graph from file
Graph load_graph(const std::string& path_to_graph, Logger& logger) {
    std::ifstream fin(path_to_graph);
    if (!fin.is_open()) {
        logger.println("ERROR! No graph file found by path " + path_to_graph, true);
    }
    std::size_t num_vertices, num_edges;
    fin >> num_vertices >> num_edges;
    
    if (fin.fail()) {
        logger.println("ERROR! Can't read graph file by path " + path_to_graph, true);
    }
    Edges edges(num_vertices);
    
    for(std::size_t i = 0; i < num_edges; ++i) {
        std::size_t from, to;
        double w;
        fin >> from  >> to >> w;
        
        if (fin.fail()) {
            logger.println("ERROR! Can't read graph file by path " + path_to_graph, true);
        }
        if ( w < 0 || from >= num_vertices || to >= num_vertices
            || from < 0 || to < 0) {
            logger.println("ERROR! Uncorrect graph. "
                        "Can't read graph file by path " + path_to_graph +
                        ".Error in edge " + std::to_string(from) + ' ' +
                        std::to_string(to) + ' ' + std::to_string(w), true);
        }
        
        edges[from][to] = w;
        edges[to][from] = w;
    }
    fin.close();
    return {num_vertices, num_edges, edges};
}


// finds shortest distances and ancestors from each vertex to the specified
// Dijkstra's algorithm implementation
std::vector<std::pair<double, std::size_t>> find_nearest(std::size_t vertex, Edges& edges) {
    std::priority_queue<
        std::pair<double, std::size_t>,
        std::vector<std::pair<double, std::size_t>>,
        std::greater<std::pair<double, std::size_t>>
    > q;
    q.push({0, vertex});
    std::vector<std::pair<double, std::size_t>> result(edges.size(), {INFINITY, vertex});
    result[vertex].first = 0;
    std::vector<bool> used(edges.size(), false);
    
    while (!q.empty()) {
        size_t cur = q.top().second;
        q.pop();
        if (used[cur]) continue;
        used[cur] = true;
        for (auto& [to, w]: edges[cur]) {
            if (!used[to] && result[to].first > result[cur].first + w) {
                result[to].first = result[cur].first + w;
                result[to].second = cur;
                q.push({result[to].first, to});
            }
        }
    }
    return result;
}
