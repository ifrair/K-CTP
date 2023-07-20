#include <iostream>
#include <memory>

#include "Opponents/AllOpponents.h"
#include "Strategies/AllStrategies.h"

using namespace std;


int main() {
    const string path = "../../../../../k-CTP/";
    const string graph_path = path + "graph2.txt";
    Logger::path = path + "log.txt";
    auto opponent = make_shared<RandomOpponent>(graph_path, 2);
    ComparisonStrategy strategy(graph_path);
    double result = strategy.go(opponent);
    cout << result << endl;
}
