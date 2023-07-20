#pragma once
#include<random>

#include "BaseOpponent.h"
#include "../Utiles/Graph.h"

using namespace std;


// Random setting blockages
class RandomOpponent : public BaseOpponent {
public:
    RandomOpponent(
                   const string& path_to_graph,
                   size_t max_num_blockages,
                   size_t is_deterministic = false,
                   int seed = 0
                   ):
    BaseOpponent(path_to_graph, max_num_blockages) {
        random_device rd;
        this->seed = (is_deterministic ? seed : rd());
    }

private:
    void set_blockages() override {
        gen = mt19937(seed);
        while (true) {
            blockages.clear();
            blockages.resize(graph.num_vertices);
            
            for (size_t i = 0; i < max_num_blockages; ++i) {
                uniform_int_distribution<size_t> dist(1, (graph.num_edges - i) * 2);
                size_t edge_num = dist(gen);
                
                for (size_t from = 0; from < graph.num_vertices; ++from) {
                    size_t num_adjacent = graph.edges[from].size() - blockages[from].size();
                    if (num_adjacent < edge_num) {
                        edge_num -= num_adjacent;
                        continue;
                    }
                    for (auto [to, _]: graph.edges[from]) {
                        if (blockages[from].contains(to)) continue;
                        --edge_num;
                        if (edge_num == 0) {
                            blockages[from].insert(to);
                            blockages[to].insert(from);
                            break;
                        }
                    }
                    if (edge_num == 0) break;
                }
            }
            
            // checking connectivity
            auto distances = graph.bfs(0, blockages);
            if (distances[graph.num_vertices - 1] != SIZE_MAX) break;
        }
    }
    
private:
    mt19937 gen;
    int seed;
};
