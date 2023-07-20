#pragma once
#include <vector>
#include <unordered_set>

#include "BaseStrategy.h"
#include "../Utiles/Graph.h"

using namespace std;


// Optimal exponential strategy
class OptimalStrategy: public BaseStrategy {
public:
    OptimalStrategy(
                    const string& path_to_graph
                    ) :
    BaseStrategy(path_to_graph) {}

private:
    double travel() override {
        size_t cur = 0;
        vector<bool> used(graph.num_vertices, false);
        double result = 0;
        while (cur != graph.num_vertices - 1) {
            used[cur] = true;
            go_to_vertex(cur);
            
            double max_path = 0;
            for (auto& adjacents: graph.edges) {
                for (auto& [to, w]: adjacents) {
                    max_path += w;
                }
            }
            if (max_path - result <= 0) {
                logger.println("Error! Too long travel!", true);
            }
            size_t to = get<1>(graph.get_optimal(
                            cur,
                            num_blockages_left,
                            max_path - result,
                            used
                            ));
            if (!graph.edges[cur].contains(to)) {
                logger.println("ERROR! No such edge: " + to_string(cur) + "->" + to_string(to), true);
            }
            result += graph.edges[cur][to];
            cur = to;
        }
        go_to_vertex(cur);
        return result;
    }
};
