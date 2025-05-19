#include "Route.h"
#include <algorithm>
#include <queue>

namespace routing
{
	// --- Node class ---
	Node::Node(int id) : m_id(id) {}

	// node manipulation
	void Node::addPredecessor(int nodeId, int requestId)	{ m_predecessors[nodeId].push_back(requestId); }
	void Node::addSuccessor(int nodeId, int requestId) { m_successors[nodeId].push_back(requestId);	}
	void Node::removePredecessor(int nodeId, int requestId) { removeRule(m_predecessors, nodeId, requestId); }
	void Node::removeSuccessor(int nodeId, int requestId) { removeRule(m_successors, nodeId, requestId); }
	
	void Node::removeRule(std::unordered_map<int, std::vector<int>>& ruleMap, int nodeId, int requestId)
	{
		auto it = ruleMap.find(nodeId);
		if (it != ruleMap.end())
		{
			auto& requestIds = it->second;
			requestIds.erase(std::remove(requestIds.begin(), requestIds.end(), requestId), requestIds.end());
			if (requestIds.empty()) ruleMap.erase(it);
		}
	}

	// node checking
	bool Node::hasPredecessors() const { return !m_predecessors.empty(); }
	bool Node::hasSuccessors() const { return !m_successors.empty(); }
	bool Node::hasDependencies() const { return hasPredecessors() || hasSuccessors(); }
	bool Node::isPredecessor(int node) const { return m_predecessors.find(node) != m_predecessors.end(); }
	bool Node::isSuccessor(int node) const { return m_successors.find(node) != m_successors.end(); }
	bool Node::isDependent(int node) const { return isPredecessor(node) || isSuccessor(node); }

	// overloaded operators

    std::ostream& operator<<(std::ostream& os, const Node& node)
    {
        os << node.m_id;
        return os;
    }

	bool Node::operator==(const Node& other) const { return m_id == other.m_id;	}

	// getters
	int Node::getId() const { return m_id; }
	const std::unordered_map<int, std::vector<int>>& Node::getPredecessors() const { return m_predecessors; }
	const std::unordered_map<int, std::vector<int>>& Node::getSuccessors() const { return m_successors; }

	// --- Route class ---
	bool Route::isValidRequest(const Request& request) const
	{
		if (request.pickupNodeId < 0 || request.deliveryNodeId < 0) return false;
		if (request.pickupNodeId == request.deliveryNodeId) return false;
		if (m_requests.find(request.id) != m_requests.end()) return false;
		return true;
	}

    bool Route::addRequest(const Request& request)  
    {  
		if (!isValidRequest(request)) return false;

		int pickupNodeId = request.pickupNodeId;
		int deliveryNodeId = request.deliveryNodeId;

		auto pickupIt = m_nodes.find(pickupNodeId);
		auto deliveryIt = m_nodes.find(deliveryNodeId);

		bool pickupExists = pickupIt != m_nodes.end();
		bool deliveryExists = deliveryIt != m_nodes.end();

		if (pickupExists && deliveryExists)
		{
			if (pickupIt->second->isPredecessor(deliveryNodeId) || deliveryIt->second->isSuccessor(pickupNodeId))
				return false;

			pickupIt->second->addSuccessor(deliveryNodeId, request.id);
			deliveryIt->second->addPredecessor(pickupNodeId, request.id);
		}
		else if (!pickupExists && !deliveryExists)
		{
			m_nodes[pickupNodeId] = std::make_unique<Node>(pickupNodeId);
			m_nodes[pickupNodeId]->addSuccessor(deliveryNodeId, request.id);

			m_nodes[deliveryNodeId] = std::make_unique<Node>(deliveryNodeId);
			m_nodes[deliveryNodeId]->addPredecessor(pickupNodeId, request.id);
		}
		else
		{
			if (!pickupExists)
			{
				m_nodes[pickupNodeId] = std::make_unique<Node>(pickupNodeId);
				m_nodes[pickupNodeId]->addSuccessor(deliveryNodeId, request.id);
				deliveryIt->second->addPredecessor(pickupNodeId, request.id);
			}
			if (!deliveryExists)
			{
				m_nodes[deliveryNodeId] = std::make_unique<Node>(deliveryNodeId);
				m_nodes[deliveryNodeId]->addPredecessor(pickupNodeId, request.id);
				pickupIt->second->addSuccessor(deliveryNodeId, request.id);
			}
		}

		m_requests[request.id] = std::make_unique<Request>(request);
		return true;
    }

    void Route::removeRequest(int requestId)
    {
        auto requestIt = m_requests.find(requestId);
        if (requestIt != m_requests.end())
        {
            const Request& req = *(requestIt->second);
            int pickupNodeId = req.pickupNodeId;
            int deliveryNodeId = req.deliveryNodeId;

            auto pickupIt = m_nodes.find(pickupNodeId);
            auto deliveryIt = m_nodes.find(deliveryNodeId);

            if (pickupIt != m_nodes.end())
            {
                pickupIt->second->removeSuccessor(deliveryNodeId, requestId);
                if (!pickupIt->second->hasDependencies())
                    m_nodes.erase(pickupIt);
            }

            if (deliveryIt != m_nodes.end())
            {
                deliveryIt->second->removePredecessor(pickupNodeId, requestId);
                if (!deliveryIt->second->hasDependencies())
                    m_nodes.erase(deliveryIt);
            }

            m_requests.erase(requestIt);
        }
    }

	std::vector<int> Route::getTopologicalOrder() const
	{
		std::unordered_map<int, int> inDegree;
		std::unordered_map<int, std::vector<int>> adjList;

		// Passo 1: Construir grafo (adjList) e calcular grau de entrada
		for (const auto& [nodeId, nodePtr] : m_nodes)
		{
			if (!inDegree.count(nodeId))
				inDegree[nodeId] = 0; // inicializa

			for (const auto& [succId, _] : nodePtr->getSuccessors())
			{
				adjList[nodeId].push_back(succId);
				inDegree[succId]++;
			}
		}

		// Passo 2: Inicializar fila com nós com grau de entrada 0
		std::queue<int> q;
		for (const auto& [nodeId, degree] : inDegree)
		{
			if (degree == 0)
				q.push(nodeId);
		}

		std::vector<int> result;

		// Passo 3: Processar ordenação topológica
		while (!q.empty())
		{
			int current = q.front();
			q.pop();
			result.push_back(current);

			for (int neighbor : adjList[current])
			{
				if (--inDegree[neighbor] == 0)
					q.push(neighbor);
			}
		}

		// Passo 4: Verificar se houve ciclo (grafo não é DAG)
		if (result.size() != m_nodes.size())
		{
			throw std::runtime_error("Ciclo detectado na rota! Não é possível obter uma ordenação topológica.");
		}

		return result;
	}


    std::ostream& operator<<(std::ostream& os, const Route& route)
    {
        using io::operator<<;
		for (const auto& pair : route.m_nodes)
		{
			os << *pair.second << " ";
		}
        return os;
    }
}