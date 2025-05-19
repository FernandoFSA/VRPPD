#pragma once
#include <vector>
#include "RouteGraph.h"

class RouteSolution {
public:
    RouteSolution(const RouteGraph& graph, std::vector<int> initialSequence);

    bool applySwap(int i, int j);
    bool applyInsertion(int from, int to);
    const std::vector<int>& getSequence() const;
    bool isFeasible() const;

private:
    const RouteGraph& m_graph;
    std::vector<int> m_sequence;
};
