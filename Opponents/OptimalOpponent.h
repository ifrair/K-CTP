#pragma once
#include<random>

#include "BaseOpponent.h"
#include "../Utiles/Graph.h"

using namespace std;


// Optimal setting blockages
// Full exponential enumeration
class OptimalOpponent : public BaseOpponent {
public:
    OptimalOpponent(
                   const string& path_to_graph,
                   size_t max_num_blockages
                   ):
    BaseOpponent(path_to_graph, max_num_blockages) {}

private:
    void set_vertex_blockages(size_t vertex) override {
        double max_path = 0;
        for (auto& adjacents: graph.edges) {
            for (auto& [to, w]: adjacents) {
                max_path += w;
            }
        }
        auto vertex_blockages = get<2>(graph.get_optimal(
                                                         vertex,
                                                         num_blockages_left,
                                                         max_path - travel_distance,
                                                         used
                                                         ));
        for (auto to: vertex_blockages) {
            blockages[vertex].insert(to);
            blockages[to].insert(vertex);
        }
    }
};
