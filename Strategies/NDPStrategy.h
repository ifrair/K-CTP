#pragma once
#include <vector>
#include <unordered_set>

#include "BaseStrategy.h"
#include "../Utiles/Graph.h"

using namespace std;


// Random (k+1)-competitive strategy when graph consist of node-disjoint s-t paths
// Warning! Works on node-disjoint s-t paths only
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
        // checking graph correctness
        size_t num_2_edged_verteces = 0;
        for(size_t node = 1; node < graph.num_vertices - 1; ++ node) {
            num_2_edged_verteces += (graph.edges[node].size() == 2);
        }
        if (graph.start_num_edges - graph.edges[0].size() + 2 != graph.num_vertices ||
            graph.edges[0].size() != graph.edges[graph.num_vertices - 1].size() ||
            num_2_edged_verteces != graph.num_vertices - 2) {
            logger.println("Error! Graph does't consist of node-disjoint s-t paths", true);
        }
        
        // finding paths
        vector<pair<size_t, double>> deleted_edges;
        for (auto& [to, w]: graph.edges[0]) {
            deleted_edges.emplace_back(to, w);
        }
        graph.edges[0].clear();
        auto shortest_distances = graph.find_shortest_paths(graph.num_vertices - 1);
        
        vector<pair<double, size_t>> paths;
        for (auto& [to, w]: deleted_edges) {
            paths.emplace_back(w + shortest_distances[to].first, to);
            graph.edges[0][to] = w;
        }
        sort(paths.begin(), paths.end());
        paths.resize(min(paths.size(), num_blockages_total + 1));
        
        // going pathes
        vector<pair<double, size_t>> block_paths;
        double block_sum = 0;
        for (size_t path = 0; path < paths.size(); ++path) {
            double dist = paths[path].first;
            if (dist > 2 * (block_sum + dist) / (block_paths.size() + 1)) {
                try_paths(block_sum, block_paths, shortest_distances);
                if (cur_vertex == graph.num_vertices - 1) return;
                block_sum = 0;
                block_paths.clear();
            }
            block_paths.push_back(paths[path]);
            block_sum += dist;
        }
        if (cur_vertex != graph.num_vertices - 1)
            try_paths(block_sum, block_paths, shortest_distances);
    }
    
    void try_paths(
                   double sum,
                   vector<pair<double, size_t>>& paths,
                   const vector<pair<double, size_t>>& shortest_distances
                   ) {
        vector<bool> path_used(paths.size(), false);
        for (size_t i = paths.size(); i > 0; --i) {
            double prob_sum = 0;
            for (size_t path = 0; path < paths.size(); ++path) {
                if (path_used[path]) continue;
                double dist = paths[path].first;
                prob_sum += ((2 - i) * dist + 2 * (sum - dist)) / i / i / dist;
            }
            
            uniform_real_distribution<> dist(0, prob_sum);
            double prob_result = dist(gen);
            
            for (size_t path = 0; path < paths.size(); ++path) {
                if (path_used[path]) continue;
                double dist = paths[path].first;
                prob_result -= ((2 - i) * dist + 2 * (sum - dist)) / i / i / dist;
                if (prob_result <= 0) {
                    follow_shortest_path(paths[path].second, shortest_distances);
                    path_used[path] = true;
                    sum -= dist;
                    break;
                }
            }
        }
        
    }
    
    

private:
    mt19937 gen;
};
