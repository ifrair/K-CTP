#include <iostream>
#include <memory>

#include "Opponents/AllOpponents.h"
#include "Strategies/AllStrategies.h"

using namespace std;


int main() {
    const string path = "../../../../../k-CTP/";
    const string graph_path = path + "graph_paths.txt";
    Logger::path = path + "log.txt";
    auto opponent = make_shared<OptimalOpponent>(graph_path, 2);
    NDPStrategy strategy(graph_path);
    double result = strategy.go(opponent);
    cout << result << endl;
}
