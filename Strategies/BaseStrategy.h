#pragma once
#include <unordered_set>
#include <vector>

#include "../Opponents/BaseOpponent.h"
#include "../Utiles/Graph.h"
#include "../Utiles/Logger.h"

using namespace std;


// Base strategy functionality
// first vertex is start last one is finish
// inherit and implement travel method
class BaseStrategy {
public:
    BaseStrategy(
                 const string& path_to_graph
                 ) :
    path_to_graph(path_to_graph),
    logger(Logger::get()) {}
    
    virtual ~BaseStrategy() = default;
    
    double go(shared_ptr<BaseOpponent> opponent) {
        logger.println("\nStarting new journey:");
        
        this->opponent = opponent;
        num_blockages_left = num_blockages_total = opponent->generate_blockages();
        
        graph.load(path_to_graph, logger);
        used.clear();
        used.resize(graph.num_vertices, false);
        cur_vertex = SIZE_MAX;
        go_to_vertex(0);
        
        travel();
        if (cur_vertex != graph.num_vertices - 1)
            logger.println("Error! Ended not in the finish vertex", true);
        logger.println("Result distace is " + to_string(travel_distance));
        logger.println("Optimal distace is " + to_string(opponent->optimal_distance));
        double competitive_ratio = travel_distance / opponent->optimal_distance;
        logger.println("Competitive ratio is " + to_string(competitive_ratio));
        return competitive_ratio;
    }

protected:
    // method to re-write
    virtual void travel() = 0;
    
    void go_to_vertex(size_t vertex) {
        if (cur_vertex != SIZE_MAX) {
            if (!graph.edges[cur_vertex].contains(vertex)) {
                logger.println("ERROR! No such edge: " + to_string(cur_vertex) + "->" + to_string(vertex), true);
            }
            travel_distance += graph.edges[cur_vertex][vertex];
        }
        cur_vertex = vertex;
        
        logger.println("Go to " + to_string(vertex));
        used[vertex] = true;
        auto blockages = opponent->go_to_vertex(vertex);
        for (auto blockage: blockages) {
            graph.edges[vertex].erase(blockage);
            graph.edges[blockage].erase(vertex);
        }
        num_blockages_left -= blockages.size();
    }

protected:
    string path_to_graph;
    Logger& logger;
    
    size_t num_blockages_total;
    size_t num_blockages_left;
    Graph graph;
    vector<bool> used;
    size_t cur_vertex = SIZE_MAX;
    double travel_distance = 0;
    
    shared_ptr<BaseOpponent> opponent = nullptr;
};
