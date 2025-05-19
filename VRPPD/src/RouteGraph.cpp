#include "RouteGraph.h"

void RouteGraph::addRequest(const Request& request) {
    m_requests[request.id] = request;
    m_predecessors[request.deliveryNodeId].insert(request.pickupNodeId);
    m_successors[request.pickupNodeId].insert(request.deliveryNodeId);
}

void RouteGraph::removeRequest(int requestId) {
    auto it = m_requests.find(requestId);
    if (it == m_requests.end()) return;

    const Request& r = it->second;
    m_predecessors[r.deliveryNodeId].erase(r.pickupNodeId);
    if (m_predecessors[r.deliveryNodeId].empty()) {
        m_predecessors.erase(r.deliveryNodeId);
    }

    m_successors[r.pickupNodeId].erase(r.deliveryNodeId);
    if (m_successors[r.pickupNodeId].empty()) {
        m_successors.erase(r.pickupNodeId);
    }

    m_requests.erase(it);
}

bool RouteGraph::isSequenceFeasible(const std::vector<int>& sequence) const {
    std::unordered_map<int, int> position;
    for (size_t i = 0; i < sequence.size(); ++i) {
        position[sequence[i]] = static_cast<int>(i);
    }

    for (size_t i = 0; i < sequence.size(); ++i) {
        int node = sequence[i];
        auto it = m_predecessors.find(node);
        if (it != m_predecessors.end()) {
            for (int pred : it->second) {
                if (position.count(pred) && position[pred] > static_cast<int>(i)) {
                    return false; // Predecessor appears after
                }
            }
        }
    }

    return true;
}