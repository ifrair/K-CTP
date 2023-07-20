#include <iostream>
#include <memory>

#include "Opponents/OptimalOpponent.h"
#include "Strategies/OptimalStrategy.h"

using namespace std;


int main() {
    const string path = "../../../../../k-CTP/";
    const string graph_path = path + "graph2.txt";
    Logger::path = path + "log.txt";
    auto opponent = make_shared<OptimalOpponent>(graph_path, 2);
    OptimalStrategy strategy(graph_path);
    double result = strategy.go(opponent);
    cout << result << endl;
}
