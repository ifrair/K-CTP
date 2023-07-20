#pragma once
#include <fstream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../Utiles/Logger.h"

using Edges = std::vector<std::unordered_map<std::size_t, double>>;

class Graph {
public:
    // loading graph from file
    void load(const std::string& path_to_graph, Logger& logger) {
        std::ifstream fin(path_to_graph);
        if (!fin.is_open()) {
            logger.println("ERROR! No graph file found by path " + path_to_graph, true);
        }
        fin >> num_vertices >> num_edges;
        
        if (fin.fail()) {
            logger.println("ERROR! Can't read graph file by path " + path_to_graph, true);
        }
        edges.clear();
        edges.resize(num_vertices);
        
        for(std::size_t i = 0; i < num_edges; ++i) {
            std::size_t from, to;
            double w;
            fin >> from  >> to >> w;
            
            if (fin.fail()) {
                logger.println("ERROR! Can't read graph file by path " + path_to_graph, true);
            }
            if ( w < 0 || from >= num_vertices || to >= num_vertices
                || from < 0 || to < 0) {
                logger.println("ERROR! Uncorrect graph. "
                               "Can't read graph file by path " + path_to_graph +
                               ".Error in edge " + std::to_string(from) + ' ' +
                               std::to_string(to) + ' ' + std::to_string(w), true);
            }
            
            edges[from][to] = w;
            edges[to][from] = w;
        }
        fin.close();
    }
    
    
    // finds shortest distances and ancestors from each vertex to the specified
    // Dijkstra's algorithm implementation
    std::vector<std::pair<double, std::size_t>> find_shortest_paths(std::size_t vertex) {
        std::priority_queue<
        std::pair<double, std::size_t>,
        std::vector<std::pair<double, std::size_t>>,
        std::greater<std::pair<double, std::size_t>>
        > q;
        q.push({0, vertex});
        std::vector<std::pair<double, std::size_t>> result(num_vertices, {INFINITY, vertex});
        result[vertex].first = 0;
        std::vector<bool> used(num_vertices, false);
        
        while (!q.empty()) {
            std::size_t cur = q.top().second;
            q.pop();
            if (used[cur]) continue;
            used[cur] = true;
            for (auto& [to, w]: edges[cur]) {
                if (!used[to] && result[to].first > result[cur].first + w) {
                    result[to].first = result[cur].first + w;
                    result[to].second = cur;
                    q.push({result[to].first, to});
                }
            }
        }
        return result;
    }
    
    
    // finds minimal number of edges from each vertex to the specified
    std::vector<std::size_t> bfs(
                                 std::size_t vertex,
                                 const std::vector<std::unordered_set<std::size_t>>& blockages = {}
                                 ) {
        std::queue<std::size_t> q({vertex});
        std::vector<bool> used(num_vertices, false);
        std::vector<std::size_t> distances(num_vertices, SIZE_MAX);
        distances[vertex] = 0;
        
        while(!q.empty()) {
            std::size_t cur = q.front();
            q.pop();
            if (used[cur]) continue;
            used[cur] = true;
            for (auto& [to, _]: edges[cur]) {
                if (!used[to] &&
                    (blockages.empty() || !blockages[cur].contains(to)) &&
                    distances[cur] + 1 < distances[to]
                    ) {
                    distances[to] = distances[cur] + 1;
                    q.push(to);
                }
            }
        }
        return distances;
    }
    
    std::tuple<double, std::size_t, std::vector<std::size_t>> get_optimal(
                             std::size_t cur,
                             std::size_t num_blockages,
                             double distance_left,
                             std::vector<bool>& used
                             ) {
        if (cur == num_vertices - 1) return {0, 0, {}};
        if (distance_left <= 0) return {INFINITY, 0, {}};
        double result = -1;
        std::size_t result_to = 0;
        std::vector<std::size_t> result_blockages;
        bool old_used = used[cur];
        used[cur] = true;
        
        std::vector<std::size_t> potential_blockages_to;
        std::vector<double> potential_blockages_w;
        for (auto& [to, w]: edges[cur]) {
            if (!old_used && !used[to]) {
                potential_blockages_to.push_back(to);
                potential_blockages_w.push_back(w);
            }
        }
        for (std::size_t mask = 0; mask < (1 << potential_blockages_to.size()); ++mask) {
            std::size_t num_bits = __builtin_popcount(mask);
            if (num_bits > num_blockages) continue;
            
            std::vector<std::size_t> blockages;
            num_blockages -= num_bits;
            for (std::size_t bit = 0; bit < potential_blockages_to.size(); ++bit) {
                if ((1 << bit) & mask) {
                    std::size_t to = potential_blockages_to[bit];
                    blockages.push_back(to);
                    edges[cur].erase(to);
                    edges[to].erase(cur);
                }
            }
            
            std::size_t mask_result_to = 0;
            double mask_result = INFINITY;
            for (auto& [to, w]: edges[cur]) {
                auto res = get_optimal(
                                       to,
                                       num_blockages,
                                       distance_left - w,
                                       used
                                       );
                if (get<0>(res) + w < mask_result) {
                    mask_result = get<0>(res) + w;
                    mask_result_to = to;
                }
            }
            if (mask_result > result && mask_result < INFINITY) {
                result = mask_result;
                result_to = mask_result_to;
                result_blockages = blockages;
            }
            
            for (std::size_t bit = 0; bit < potential_blockages_to.size(); ++bit) {
                if ((1 << bit) & mask) {
                    std::size_t to = potential_blockages_to[bit];
                    double w = potential_blockages_w[bit];
                    edges[cur][to] = w;
                    edges[to][cur] = w;
                }
            }
            num_blockages += num_bits;
        }
        
        used[cur] = old_used;
        return {(result == -1 ? INFINITY : result), result_to, result_blockages};
    }
    
public:
    std::size_t num_vertices;
    std::size_t num_edges;
    Edges edges;
};
