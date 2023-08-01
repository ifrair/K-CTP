#include <iostream>
#include <memory>

#include "Opponents/AllOpponents.h"
#include "Strategies/AllStrategies.h"

using namespace std;


int main() {
    
    const string path = "../../../../../k-CTP/";
    const string graph_path = path + "graph_paths.txt";
    Logger::path = path + "log.txt";
    
//    size_t num_vertices = 10;
//    size_t num_edges_max = 15, num_blockages_max = 10;
//    int seed_min = 0, seed_max = 30;
//    int game_seed_min = 0, game_seed_max = 30;
//
//    vector<double>avg, mx, mxavg;
//    for (size_t k = 1; k <= num_blockages_max; ++k) {
//        double avg_sum = 0;
//        double max_rate = 0, max_seed = 0, max_num_edges = 0;
//        double max_avg_rate = 0, max_avg_seed = 0, max_avg_num_edges = 0;
//        size_t num_results = 0;
//
//        for (size_t num_edges = k + 5; num_edges <= num_edges_max; ++num_edges) {
//            for (int seed = seed_min; seed <= seed_max; ++seed) {
//                double result_sum = 0;
//                Graph::generate_graph(num_vertices, num_edges, 10, seed, graph_path);
//                for(int game_seed = game_seed_min; game_seed <= game_seed_max; ++game_seed) {
//                    auto opponent = make_shared<RandomOpponent>(graph_path, k, 1, game_seed);
//                    DetourStrategy strategy(graph_path);
//                    double result = strategy.go(opponent);
//                    result_sum += result;
//                    if (result > max_rate) {
//                        max_rate = result;
//                        max_seed = seed;
//                        max_num_edges = num_edges;
//                    }
//                }
//                result_sum /= (game_seed_max - game_seed_min + 1);
//                if (result_sum > max_avg_rate) {
//                    max_avg_rate = result_sum;
//                    max_avg_seed = seed;
//                    max_avg_num_edges = num_edges;
//                }
//                avg_sum += result_sum;
//                ++num_results;
//                //        cout << seed << ' ' << result << endl;
//            }
//        }
//        avg_sum /= num_results;
//        cout << "Blockages number: " << k << endl;
//        cout << "Average rate: " << avg_sum << endl;
//        cout << "Maximal average rate: " << max_avg_rate << " with seed " << max_avg_seed << " with " << max_avg_num_edges << " edges" << endl;
//        cout << "Maximal rate: " << max_rate << " with seed " << max_seed << " with " << max_num_edges << " edges" << endl << endl;
//        avg.push_back(avg_sum);
//        mx.push_back(max_rate);
//        mxavg.push_back(max_avg_rate);
//    }
//    for (auto i: avg) cout << i << ", ";cout << endl;
//    for (auto i: mx) cout << i << ", ";cout << endl;
//    for (auto i: mxavg) cout << i << ", ";cout << endl;
    
    int seed_min = 0, seed_max = 1000;
    int game_seed_min = 421, game_seed_max = 421;
    size_t k = 5;

    
    double avg_sum = 0;
    double max_rate = 0, max_seed = 0;
    double max_avg_rate = 0, max_avg_seed = 0;
    size_t num_results = 0;
    
    for (int seed = seed_min; seed <= seed_max; ++seed) {
        double result_sum = 0;
        for(int game_seed = game_seed_min; game_seed <= game_seed_max; ++game_seed) {
            auto opponent = make_shared<RandomOpponent>(graph_path, k, 1, game_seed);
            GRRStrategy strategy(graph_path, 1, seed);
            double result = strategy.go(opponent);
            result_sum += result;
            if (result > max_rate) {
                max_rate = result;
                max_seed = seed;
            }
        }
        result_sum /= (game_seed_max - game_seed_min + 1);
        if (result_sum > max_avg_rate) {
            max_avg_rate = result_sum;
            max_avg_seed = seed;
        }
        avg_sum += result_sum;
        ++num_results;
    }
    avg_sum /= num_results;
    cout << "Blockages number: " << k << endl;
    cout << "Average rate: " << avg_sum << endl;
    cout << "Maximal average rate: " << max_avg_rate << " with seed " << max_avg_seed << endl;
    cout << "Maximal rate: " << max_rate << " with seed " << max_seed <<  endl << endl;
}
