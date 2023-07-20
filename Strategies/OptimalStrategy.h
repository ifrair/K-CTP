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
    void travel() override {
        while (cur_vertex != graph.num_vertices - 1) {
            double max_path = 0;
            for (auto& adjacents: graph.edges) {
                for (auto& [to, w]: adjacents) {
                    max_path += w;
                }
            }
            if (max_path - travel_distance <= 0) {
                logger.println("Error! Too long travel!", true);
            }
            size_t to = get<1>(graph.get_optimal(
                            cur_vertex,
                            num_blockages_left,
                            max_path - travel_distance,
                            used
                            ));
            if (!graph.edges[cur_vertex].contains(to)) {
                logger.println("ERROR! No such edge: " + to_string(cur_vertex) + "->" + to_string(to), true);
            }
            travel_distance += graph.edges[cur_vertex][to];
            cur_vertex = to;
            go_to_vertex(cur_vertex);
        }
    }
};
