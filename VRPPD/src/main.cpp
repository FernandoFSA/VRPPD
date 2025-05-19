#include <iostream>
#include "RouteGraph.h"
#include "RouteSolution.h"

int main() {
    RouteGraph graph;
    graph.addRequest({ 1, 1, 3 });
    graph.addRequest({ 2, 2, 4 });
    graph.addRequest({ 3, 3, 5 });

    RouteSolution solution(graph, { 1, 2, 3, 4, 5 });

    std::cout << "Original: ";
    for (int n : solution.getSequence()) std::cout << n << " ";
    std::cout << "\nFeasible? " << solution.isFeasible() << "\n";

    std::cout << "Trying swap(2, 3)... ";
    if (solution.applySwap(2, 3)) {
        std::cout << "Accepted!\n";
    }
    else {
        std::cout << "Rejected (precedence violation)\n";
    }

    std::cout << "Final: ";
    for (int n : solution.getSequence()) std::cout << n << " ";
    std::cout << "\n";
}
