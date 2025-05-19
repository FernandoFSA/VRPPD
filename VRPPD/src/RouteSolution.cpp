#include "RouteSolution.h"
#include <algorithm>

RouteSolution::RouteSolution(const RouteGraph& graph, std::vector<int> initialSequence)
    : m_graph(graph), m_sequence(std::move(initialSequence)) {
}

bool RouteSolution::isFeasible() const {
    return m_graph.isSequenceFeasible(m_sequence);
}

bool RouteSolution::applySwap(int i, int j) {
    if (i == j) return true;
    std::swap(m_sequence[i], m_sequence[j]);
    if (isFeasible()) return true;
    std::swap(m_sequence[i], m_sequence[j]); // rollback
    return false;
}

bool RouteSolution::applyInsertion(int from, int to) {
    if (from == to) return true;

    int node = m_sequence[from];
    m_sequence.erase(m_sequence.begin() + from);
    m_sequence.insert(m_sequence.begin() + (to < from ? to : to - 1), node);

    if (isFeasible()) return true;

    // rollback
    m_sequence.erase(m_sequence.begin() + (to < from ? to : to - 1));
    m_sequence.insert(m_sequence.begin() + from, node);
    return false;
}

const std::vector<int>& RouteSolution::getSequence() const {
    return m_sequence;
}