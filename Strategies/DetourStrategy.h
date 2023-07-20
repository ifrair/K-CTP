#pragma once
#include <vector>
#include <unordered_set>

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
        
    }
};
