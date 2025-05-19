#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Request.h"

class RouteGraph {
public:
    void addRequest(const Request& request);
    void removeRequest(int requestId);
    bool isSequenceFeasible(const std::vector<int>& sequence) const;

private:
    std::unordered_map<int, std::unordered_set<int>> m_predecessors; // nodeId -> predecessors
    std::unordered_map<int, std::unordered_set<int>> m_successors;   // nodeId -> successors
    std::unordered_map<int, Request> m_requests;
};