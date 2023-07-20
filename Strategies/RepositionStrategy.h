#pragma once
#include <stack>
#include <vector>
#include <unordered_set>

#include "BaseStrategy.h"
#include "../Utiles/Graph.h"

using namespace std;


// Reposition deterministic (2 * k + 1)-competitive universal strategy
class RepositionStrategy: public BaseStrategy {
public:
    RepositionStrategy(
                   const string& path_to_graph
                   ) :
    BaseStrategy(path_to_graph) {}

private:
    void travel() override {
        while (cur_vertex != graph.num_vertices - 1) {
            auto shortest_paths = graph.find_shortest_paths(graph.num_vertices - 1);
            stack<size_t> path;
            while (cur_vertex != graph.num_vertices - 1) {
                path.push(cur_vertex);
                size_t to = shortest_paths[cur_vertex].second;
                if (!graph.edges[cur_vertex].contains(to)) {
                    break;
                }
                go_to_vertex(to);
            }
            if (cur_vertex != graph.num_vertices - 1) {
                path.pop();
                while (!path.empty()) {
                    size_t to = path.top();
                    path.pop();
                    go_to_vertex(to);
                }
            }
        }
    }
};
