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
    ) : BaseStrategy(path_to_graph) {}

private:
    double travel() override {
        for(size_t i = 0; i < graph.num_vertices; ++i) {
            go_to_vertex(i);
        }
        return 0;
    }
};
