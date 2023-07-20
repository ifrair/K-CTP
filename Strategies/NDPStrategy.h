#pragma once
#include <vector>
#include <unordered_set>

#include "BaseStrategy.h"
#include "../Utiles/Graph.h"

using namespace std;


// Random (k+1)-competitive strategy when graph consist of node-disjoint s-t paths
class NDPStrategy: public BaseStrategy {
public:
    NDPStrategy(
                const string& path_to_graph,
                size_t is_deterministic = false,
                int seed = 0
                ) :
    BaseStrategy(path_to_graph) {
        random_device rd;
        gen = mt19937(is_deterministic ? seed : rd());
    }

private:
    void travel() override {
        
    }
private:
    mt19937 gen;
};
