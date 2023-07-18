#pragma once
#include<random>
#include<queue>

#include "BaseOpponent.h"

using namespace std;


// Random setting blockages
class RandomOpponent : public BaseOpponent {
public:
    RandomOpponent(
                   const string& path_to_graph,
                   size_t max_num_blockages,
                   size_t is_deterministic = false
    ) : BaseOpponent(path_to_graph, max_num_blockages) {
        random_device rd;
        gen = mt19937(is_deterministic ? 0 : rd());
    }

private:
    void set_blockages() override {
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
            queue<size_t> q({0});
            vector<bool> used(graph.num_vertices, false);
            while(!q.empty()) {
                size_t cur = q.front();
                q.pop();
                if (used[cur]) continue;
                used[cur] = true;
                for (auto& [to, _]: graph.edges[cur]) {
                    if (!used[to] && !blockages[cur].contains(to)) q.push(to);
                }
            }
            if (used[graph.num_vertices - 1]) break;
        }
    }
    
private:
    mt19937 gen;
};
