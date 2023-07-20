#pragma once
#include <vector>
#include <unordered_set>

#include "BaseStrategy.h"
#include "../Utiles/Graph.h"

using namespace std;


// GRR random ((1 + sqrt(2) / 2) * k + sqrt(2))-competitive universal strategy
class GRRStrategy: public BaseStrategy {
public:
    GRRStrategy(
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
