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
        num_blockages_total = opponent->generate_blockages();
        num_blockages_made = 0;
        
        graph = load_graph(path_to_graph, logger);
        
        return travel();
    }

protected:
    // method to re-write
    virtual double travel() = 0;
    
    unordered_set<size_t> go_to_vertex(size_t vertex) {
        logger.println("Go to " + to_string(vertex));
        auto blockages = opponent->go_to_vertex(vertex);
        for (auto blockage: blockages) {
            graph.edges[vertex].erase(blockage);
            graph.edges[blockage].erase(vertex);
        }
        return blockages;
    }

protected:
    string path_to_graph;
    Logger& logger;
    
    size_t num_blockages_total;
    size_t num_blockages_made;
    Graph graph;
    
    shared_ptr<BaseOpponent> opponent = nullptr;
};
