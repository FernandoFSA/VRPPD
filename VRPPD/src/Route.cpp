#include "Route.h"
#include <algorithm>

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

	// --- Route class ---
    bool Route::isValidRequest(const Request& request) const
    {
        if (request.pickupNodeId < 0 || request.deliveryNodeId < 0) return false; // Invalid request
        if (request.pickupNodeId == request.deliveryNodeId) return false; // Invalid request

        // Compare the dereferenced unique_ptr objects with the request
        auto it = std::find_if(m_requests.begin(), m_requests.end(), [&request](const std::unique_ptr<Request>& reqPtr) {
            return *reqPtr == request;
            });

        if (it != m_requests.end()) return false; // Request already exists
        return true;
    }

    bool Route::addRequest(const Request& request)  
    {  
       if (!isValidRequest(request)) return false;  

       int pickupNodeId = request.pickupNodeId;  
       int deliveryNodeId = request.deliveryNodeId;  
       auto pickupNodeIt = std::find_if(m_nodes.begin(), m_nodes.end(), [pickupNodeId](const std::unique_ptr<Node>& node) { return node->getId() == pickupNodeId; });  
       auto deliveryNodeIt = std::find_if(m_nodes.begin(), m_nodes.end(), [deliveryNodeId](const std::unique_ptr<Node>& node) { return node->getId() == deliveryNodeId; });  
       bool isPickupNodePresent = (pickupNodeIt != m_nodes.end());  
       bool isDeliveryNodePresent = (deliveryNodeIt != m_nodes.end());  

       // If both nodes are present and in the correct order, add the request  
       if (isPickupNodePresent && isDeliveryNodePresent)
       {  
           if ((*pickupNodeIt)->isPredecessor(deliveryNodeId) || (*deliveryNodeIt)->isSuccessor(pickupNodeId))
           {  
               return false; // Invalid request due to opposite dependencies  
           }  
           (*pickupNodeIt)->addSuccessor(deliveryNodeId, request.id);  
           (*deliveryNodeIt)->addPredecessor(pickupNodeId, request.id);  
       }  
       else if (!isPickupNodePresent && !isDeliveryNodePresent)  
       {  
           m_nodes.emplace_back(std::make_unique<Node>(pickupNodeId));  
           m_nodes.back()->addSuccessor(deliveryNodeId, request.id);  
           m_nodes.emplace_back(std::make_unique<Node>(deliveryNodeId));  
           m_nodes.back()->addPredecessor(pickupNodeId, request.id);  
       }  
       else  
       {  
           // Add the missing node and update the existing one  
           if (!isPickupNodePresent)  
           {  
               (*deliveryNodeIt)->addPredecessor(pickupNodeId, request.id);
               m_nodes.insert(m_nodes.begin(), std::make_unique<Node>(pickupNodeId));  
               m_nodes.front()->addSuccessor(deliveryNodeId, request.id);  
           }  
           if (!isDeliveryNodePresent)  
           {  
               (*pickupNodeIt)->addSuccessor(deliveryNodeId, request.id);  
               m_nodes.emplace_back(std::make_unique<Node>(deliveryNodeId));  
               m_nodes.back()->addPredecessor(pickupNodeId, request.id);  
           }  
       }  
       m_requests.push_back(std::make_unique<Request>(request));

       return true;  
    }

    void Route::removeRequest(int requestId)
    {
       auto requestIt = std::find_if(m_requests.begin(), m_requests.end(), [requestId](const std::unique_ptr<Request>& request) { return request->id == requestId; });
       auto pickupNodeIt = std::find_if(m_nodes.begin(), m_nodes.end(), [requestIt](const std::unique_ptr<Node>& node) { return node->getId() == (*requestIt)->pickupNodeId; });
	   auto deliveryNodeIt = std::find_if(m_nodes.begin(), m_nodes.end(), [requestIt](const std::unique_ptr<Node>& node) { return node->getId() == (*requestIt)->deliveryNodeId; });

       if (requestIt != m_requests.end())
       {
		   (*deliveryNodeIt)->removePredecessor((*requestIt)->pickupNodeId, requestId);
           if (!(*deliveryNodeIt)->hasDependencies()) m_nodes.erase(deliveryNodeIt);
		   (*pickupNodeIt)->removeSuccessor((*requestIt)->deliveryNodeId, requestId);
           if (!(*pickupNodeIt)->hasDependencies()) m_nodes.erase(pickupNodeIt);
           m_requests.erase(requestIt);
       }
    }

    std::ostream& operator<<(std::ostream& os, const Route& route)
    {
        using io::operator<<;
		for (const auto& node : route.m_nodes)
		{
			os << *node << " ";
		}
        return os;
    }
}