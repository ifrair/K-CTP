#pragma once
#include <cmath>
#include <stack>

#include "BaseStrategy.h"
#include "../Utiles/Graph.h"

using namespace std;


// Detour deterministic min(2 * k + 1, (2 * q + sqrt(2) * (k - q))-competitive universal strategy
// where q is maximum (s,t)-cut size
class DetourStrategy: public BaseStrategy {
public:
    DetourStrategy(
                   const string& path_to_graph
                   ) :
    BaseStrategy(path_to_graph) {}

private:
    void travel() override {
        auto shortest_paths = graph.find_shortest_paths(graph.num_vertices - 1);
        stack<size_t> path;
        double w0 = 0;
        while (cur_vertex != graph.num_vertices - 1) {
            if (cur_vertex == 0) w0 = shortest_paths[0].first;
            while (cur_vertex != graph.num_vertices - 1) {
                path.push(cur_vertex);
                size_t to = shortest_paths[cur_vertex].second;
                if (!graph.edges[cur_vertex].contains(to)) {
                    shortest_paths = graph.find_shortest_paths(graph.num_vertices - 1);
                    break;
                }
                go_to_vertex(to);
            }
            if (cur_vertex != graph.num_vertices - 1) {
                while (!path.empty()) {
                    size_t to = path.top();
                    path.pop();
                    if (shortest_paths[cur_vertex].first <= w0 / sqrt(2)) {
                        break;
                    }
                    if (to != cur_vertex) {
                        go_to_vertex(to);
                    }
                }
            }
        }
    }
};
