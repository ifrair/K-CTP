#include <iostream>
#include <memory>

#include "Opponents/RandomOpponent.h"
#include "Strategies/OptimalStrategy.h"

using namespace std;


int main() {
    const string path = "../../../../../k-CTP/";
    Logger::path = path + "log.txt";
    auto opponent = make_shared<RandomOpponent>(path + "graph.txt", 12);
    OptimalStrategy strategy(path + "graph.txt");
    double result = strategy.go(opponent);
    
}
