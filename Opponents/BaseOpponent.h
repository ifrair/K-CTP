#pragma once
#include <vector>
#include <unordered_set>

#include "../Utiles/Graph.h"
#include "../Utiles/Logger.h"

using namespace std;


// Base opponent functionality
// inherit and implement set_blockages/set_vertex_blockages method
class BaseOpponent {
public:
    BaseOpponent(
                 const string& path_to_graph,
                 size_t max_num_blockages
                 ) :
    path_to_graph(path_to_graph),
    max_num_blockages(max_num_blockages),
    logger(Logger::get()) {}
    
    virtual ~BaseOpponent() = default;
    
    size_t generate_blockages() {
        travel_distance = 0;
        cur_vertex = SIZE_MAX;
        graph.load(path_to_graph, logger);
        used.clear();
        used.resize(graph.num_vertices, false);
        blockages.clear();
        blockages.resize(graph.num_vertices);
        auto distanсes = graph.bfs(0);
        num_blockages_left = max_num_blockages = min(
                                                     max_num_blockages,
                                                     graph.num_edges - distanсes[graph.num_vertices - 1]
                                                     );
        logger.println("Generating " + to_string(max_num_blockages) + " blockages");

        set_blockages();
        Graph end_graph = graph;
        for (size_t from = 0; from < graph.num_vertices; ++from) {
            for (auto to: blockages[from]) {
                end_graph.edges[from].erase(to);
                end_graph.edges[to].erase(from);
            }
        }
        auto shortest_paths = end_graph.find_shortest_paths(graph.num_vertices - 1);
        optimal_distance = shortest_paths[0].first;
        return max_num_blockages;
    }
    
    unordered_set<size_t> go_to_vertex(size_t vertex) {
        if (cur_vertex != SIZE_MAX) {
            if (!graph.edges[cur_vertex].contains(vertex)) {
                logger.println("ERROR! No such edge: " + to_string(cur_vertex) + "->" + to_string(vertex), true);
            }
            travel_distance += graph.edges[cur_vertex][vertex];
        }
        cur_vertex = vertex;
        
        set_vertex_blockages(vertex);
        auto vertex_blockages = blockages[vertex];
        for (auto blockage: vertex_blockages) {
            logger.println("Edge " + to_string(vertex) + "<->" +
                           to_string(blockage) + " is blocked!");
            blockages[vertex].erase(blockage);
            blockages[blockage].erase(vertex);
            graph.edges[vertex].erase(blockage);
            graph.edges[blockage].erase(vertex);
        }
        num_blockages_left -= vertex_blockages.size();
        used[vertex] = true;
        return vertex_blockages;
    }
    
    size_t get_max_num_blockages() const{
        return max_num_blockages;
    }

protected:
    // method to re-write on oblivious opponent
    virtual void set_blockages() {}
    
    // method to re-write on other cases
    virtual void set_vertex_blockages(size_t vertex) {}

public:
    double optimal_distance;
    
protected:
    string path_to_graph;
    Logger& logger;
    
    size_t max_num_blockages;
    size_t num_blockages_left;
    Graph graph;
    vector<unordered_set<size_t>> blockages;
    double travel_distance;
    size_t cur_vertex;
    vector<bool> used;
};
