#pragma once
#include<random>

#include "BaseOpponent.h"
#include "../Utiles/Graph.h"

using namespace std;


// Setting blockages from file
class FixedOpponent : public BaseOpponent {
public:
    FixedOpponent(
                   const string& path_to_graph,
                   const string& path_to_blockages
                   ):
    BaseOpponent(path_to_graph),
    path_to_blockages(path_to_blockages) {
        max_num_blockages = get_num_blockages();
    }

private:
    size_t get_num_blockages() const {
        std::ifstream fin(path_to_blockages);
        if (!fin.is_open()) {
            logger.println("ERROR! No graph file found by path " + path_to_graph, true);
        }
        size_t num_blockages;
        fin >> num_blockages;
        if (fin.fail()) {
            logger.println("ERROR! Can't read graph file by path " + path_to_graph, true);
        }
        fin.close();
        return num_blockages;
    }
    
    void set_blockages() override {
        std::ifstream fin(path_to_blockages);
        if (!fin.is_open()) {
            logger.println("ERROR! No blockages file found by path " + path_to_blockages, true);
        }
        size_t num_blockages;
        fin >> num_blockages;
        if (fin.fail()) {
            logger.println("ERROR! Can't read blockages file by path " + path_to_blockages, true);
        }
        for (int blockage_num = 0; blockage_num < num_blockages; ++blockage_num) {
            size_t from, to;
            fin >> from >> to;
            if (fin.fail()) {
                logger.println("ERROR! Can't read blockages file by path " + path_to_blockages, true);
            }
            blockages[from].insert(to);
            blockages[to].insert(from);
        }
        fin.close();
    }
    
private:
    const string path_to_blockages;
};
