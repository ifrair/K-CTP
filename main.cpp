#include <iostream>
#include <memory>

#include "Opponents/AllOpponents.h"
#include "Strategies/AllStrategies.h"

using namespace std;


int main() {
    const string path = "../../../../../k-CTP/";
    const string graph_path = path + "graph.txt";
    Logger::path = path + "log.txt";
    auto opponent = make_shared<RandomOpponent>(graph_path, 9, 1, 1);
    GRRStrategy strategy(graph_path, 1, 5);
    double result = strategy.go(opponent);
    cout << result << endl;
}
