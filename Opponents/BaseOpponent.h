#pragma once
#include <vector>
#include <unordered_set>

#include "../Utiles/Graph.h"
#include "../Utiles/Logger.h"

using namespace std;


// Base opponent functionality
// inherit and implement set_blockages method
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
        logger.println("Generating " + to_string(max_num_blockages) + " blockages");
        graph = load_graph(path_to_graph, logger);

        set_blockages();
        return max_num_blockages;
    }
    
    unordered_set<size_t> go_to_vertex(size_t vertex) {
        auto vertex_blockages = blockages[vertex];
        for (auto blockage: vertex_blockages) {
            logger.println("Edge " + to_string(vertex) + "<->" +
                           to_string(blockage) + " is blocked!");
            blockages[vertex].erase(blockage);
            blockages[blockage].erase(vertex);
        }
        return vertex_blockages;
    }
    
    size_t get_max_num_blockages() const{
        return max_num_blockages;
    }

protected:
    // method to re-write
    virtual void set_blockages() = 0;
    
protected:
    string path_to_graph;
    Logger& logger;
    
    size_t max_num_blockages;
    Graph graph;
    vector<unordered_set<size_t>> blockages;
};
