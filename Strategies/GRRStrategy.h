#pragma once
#include <vector>
#include <set>
#include <unordered_set>

#include "BaseStrategy.h"
#include "../Utiles/Graph.h"

using namespace std;


// Greedy and reposition randomized ((1 + sqrt(2) / 2) * k + sqrt(2))-competitive
// pseudo-polynomial universal strategy
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
        while (cur_vertex != graph.num_vertices - 1) {
            auto shortest_distances = graph.find_shortest_paths(graph.num_vertices - 1);
            if (!num_blockages_left) {
                follow_shortest_path(0, shortest_distances);
            }
            uniform_int_distribution<size_t> dist(1, num_blockages_left + 1);
            size_t rand_res = dist(gen);
            if (rand_res == 1) {
                auto response = create_tree((1 + alpha) * shortest_distances[0].first);
                auto tree = response.first;
                auto vertex_mapping = response.second;
                traverse_tree(tree, vertex_mapping);
            } else {
                follow_shortest_path(0, shortest_distances);
            }
        }
    }
    
    pair<Graph, vector<size_t>> create_tree(double max_path_length) {
        // getting some shortest distances to each vertex
        double edges_sum = 0;
        size_t num_edges = 0;
        for (auto& adjacents: graph.edges) {
            for (auto [_, w]: adjacents) {
                edges_sum += w;
                ++num_edges;
            }
        }
        num_edges /= 2;
        edges_sum /= 2;
        
        unordered_set<size_t> vertex_q({graph.num_vertices - 1});
        vector<set<double>> shortest_distances(graph.num_vertices);
        shortest_distances[graph.num_vertices - 1].insert(0);
        edges_sum = (int)(min(edges_sum, max_path_length) * pow(10, precision)) + 1;
        for (size_t i = 0; i < num_edges * edges_sum; ++i) {
            unordered_set<size_t> new_vertex_q;
            vector<set<double>> new_shortest_distances(graph.num_vertices);
            for (size_t to: vertex_q) {
                for (auto [from, w]: graph.edges[to]) {
                    if (from == graph.num_vertices - 1) continue;
                    new_vertex_q.insert(from);
                    for (auto dist: shortest_distances[to]) {
                        if (dist + w <= max_path_length) {
                            new_shortest_distances[from].insert(dist + w);
                            if (new_shortest_distances[from].size() > edges_sum) {
                                auto it = new_shortest_distances[from].end();
                                --it;
                                new_shortest_distances[from].erase(it);
                            }
                        }
                    }
                }
            }
            for (size_t node = 0; node < graph.num_vertices; ++node) {
                shortest_distances[node].merge(new_shortest_distances[node]);
            }
            vertex_q = new_vertex_q;
        }
        
        // implicit tree representation
        Graph tree(shortest_distances[0].size());
        vector<set<double>> achived_distances(graph.num_vertices);
        achived_distances[0] = shortest_distances[0];
        vector<unordered_map<double, size_t>> length_mapping(graph.num_vertices);
        size_t node = 0;
        for (auto dist: shortest_distances[0]) {
            length_mapping[0][dist] = node++;
        }
        vector<size_t> vertex_mapping(shortest_distances[0].size(), 0);
        vertex_q.clear();
        vertex_q.insert(0);
        
        while (!vertex_q.empty()) {
            unordered_set<size_t> new_vertex_q;
            for (size_t from: vertex_q) {
                for (auto [to, w]: graph.edges[from]) {
                    set<double> dists_to;
                    for (auto dist: achived_distances[from]) {
                        dists_to.insert(dist - w);
                    }
                    set<double> dists_intersect;
                    set_intersection(
                                     shortest_distances[to].begin(),
                                     shortest_distances[to].end(),
                                     dists_to.begin(),
                                     dists_to.end(),
                                     inserter(dists_intersect, dists_intersect.begin())
                                     );
                    auto dists_intersect_copy = dists_intersect;
                    size_t old_size = achived_distances[to].size();
                    achived_distances[to].merge(dists_intersect_copy);
                    if (!dists_intersect.empty()) {
                        for (auto dist: dists_intersect) {
                            if (!length_mapping[to].contains(dist)) {
                                length_mapping[to][dist] = tree.num_vertices;
                                vertex_mapping.push_back(to);
                                ++tree.num_vertices;
                                tree.edges.push_back({});
                            }
                            tree.edges[length_mapping[from][dist + w]][length_mapping[to][dist]] = w;
                        }
                        if (achived_distances[to].size() != old_size) new_vertex_q.insert(to);
                    }
                }
            }
            
            vertex_q = new_vertex_q;
        }
        
        return {tree, vertex_mapping};
    }
    
    void traverse_tree(
                       Graph& tree,
                       const vector<size_t>& vertex_mapping
                       ) {
        size_t final_vertex = 0;
        for (size_t node = 0; node < tree.num_vertices; ++node) {
            final_vertex = (vertex_mapping[node] == graph.num_vertices - 1) ? node : final_vertex;
        }
        size_t vertex_to_visit = final_vertex;
        while (cur_vertex != graph.num_vertices - 1) {
            stack<size_t> path;
            size_t tree_cur = 0;
            auto probabilities = get_probabilities(vertex_to_visit, final_vertex, tree);
            double sum_start = 0;
            for (size_t node = 0; node < tree.num_vertices; ++node) {
                if (vertex_mapping[node] == 0) {
                    sum_start += probabilities[node];
                }
            }
            if (sum_start == 0) break;
            uniform_real_distribution<> dist(0, sum_start);
            double rand_res = dist(gen);
            for (size_t node = 0; node < tree.num_vertices; ++node) {
                if (vertex_mapping[node] == 0) {
                    rand_res -= probabilities[node];
                    if (rand_res < 0) {
                        tree_cur = node;
                        break;
                    }
                }
            }
            while (cur_vertex != graph.num_vertices - 1) {
                path.push(cur_vertex);
                auto probabilities = get_probabilities(vertex_to_visit, final_vertex, tree);
                if (probabilities[tree_cur] == 0) {
                    break;
                }
                size_t to = SIZE_MAX;
                uniform_real_distribution<> dist(0, probabilities[tree_cur]);
                double rand_res = dist(gen);
                for (auto [node, _]: tree.edges[tree_cur]) {
                    rand_res -= probabilities[node];
                    if (rand_res < 0) {
                        to = node;
                        break;
                    }
                }
                
            
                // removing blockages
                auto blockages = go_to_vertex(vertex_mapping[to]);
                tree_cur = to;
                for (size_t node = 0; node < tree.num_vertices; ++node) {
                    if (blockages.contains(vertex_mapping[node])) {
                        auto to_nodes = tree.edges[node];
                        for (auto [to_node, _]: to_nodes) {
                            if (vertex_mapping[to_node] == vertex_mapping[to]) {
                                tree.edges[node].erase(to_node);
                            } 
                        }
                    }
                    if (vertex_mapping[to] == vertex_mapping[node]) {
                        auto to_nodes = tree.edges[node];
                        for (auto [to_node, _]: to_nodes) {
                            if (blockages.contains(vertex_mapping[to_node])) {
                                tree.edges[node].erase(to_node);
                            }
                        }
                    }
                }
            }
            if (cur_vertex == graph.num_vertices - 1) return;
            path.pop();
            while (cur_vertex != 0) {
                size_t to = path.top();
                path.pop();
                go_to_vertex(to);
            }
        }
    }
    
    vector<double> get_probabilities(size_t vertex, size_t final_vertex, const Graph& tree) {
        Graph reversed_tree = tree.reverse();
        vector<double> probabilities(tree.num_vertices, 0);
        probabilities[final_vertex] = 1;
        vector<size_t> order, reversed_order;
        vector<bool> used(tree.num_vertices, false);
        dfs_order(vertex, tree, order, used);
        used.clear();
        used.resize(tree.num_vertices, false);
        dfs_order(vertex, reversed_tree, reversed_order, used);
        reverse(reversed_order.begin(), reversed_order.end());
        unordered_set<size_t> set_vertices(order.begin(), order.end());
        
        for (size_t from: order) {
            size_t num_edges = 0;
            for (auto [to, _]: reversed_tree.edges[from]) {
                if (set_vertices.contains(to)) ++num_edges;
            }
            probabilities[from] /= max(1.0, (double)num_edges);
            for (auto [to, _]: reversed_tree.edges[from]) {
                if (set_vertices.contains(to)) {
                    probabilities[to] += probabilities[from];
                }
            }
        }
        for (size_t from: reversed_order) {
            probabilities[from] /= max(1.0, (double)reversed_tree.edges[from].size());
            for (auto [to, _]: reversed_tree.edges[from]) {
                probabilities[to] += probabilities[from];
            }
        }
        return probabilities;
    }

    void dfs_order(size_t vertex, const Graph& tree, vector<size_t>& order, vector<bool>& used) {
        if(used[vertex]) return;
        used[vertex] = true;
        for (auto [to, _]: tree.edges[vertex]) {
            dfs_order(to, tree, order, used);
        }
        order.push_back(vertex);
    }

private:
    mt19937 gen;
    size_t precision = 0;
    double alpha = sqrt(2) - 1;
};
