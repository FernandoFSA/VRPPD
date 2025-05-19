#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include "OutputOperators.h"

namespace routing
{
	// --- Enums and Type Aliases ---
	enum class RuleType { Predecessor, Successor };

	// --- Request struct ---
	struct Request
	{
		const int id;
		const int pickupNodeId;
		const int deliveryNodeId;

		bool operator==(const Request& other) const {
			return id == other.id &&
				pickupNodeId == other.pickupNodeId &&
				deliveryNodeId == other.deliveryNodeId;
		}
	};

	// --- Node class ---
	class Node
	{
	public:
		Node(int id);

		// overloaded operators
		bool operator==(const Node& other) const; // for checking equality
		friend std::ostream& operator<<(std::ostream& os, const Node& node); // for printing

		// node manipulation
		void addPredecessor(int nodeId, int requestId);
		void addSuccessor(int nodeId, int requestId);
		void removePredecessor(int nodeId, int requestId);
		void removeSuccessor(int nodeId, int requestId);
		void removeRule(std::unordered_map<int, std::vector<int>>& ruleMap, int nodeId, int requestId);

		// node checking
		bool hasPredecessors() const;
		bool hasSuccessors() const;
		bool hasDependencies() const;
		bool isPredecessor(int nodeId) const;
		bool isSuccessor(int nodeId) const;
		bool isDependent(int nodeId) const;

		// getters
		int getId() const;

	private:
		const int m_id;
		std::unordered_map<int, std::vector<int>> m_predecessors; // node -> requestIds
		std::unordered_map<int, std::vector<int>> m_successors;   // node -> requestIds
	};

	// --- Route class ---
	class Route
	{
	public:
		friend std::ostream& operator<<(std::ostream& os, const Route& node); // for printing

		bool addRequest(const Request& request);
		bool isValidRequest(const Request& request) const;
		void removeRequest(int requestId);
	private:
		std::vector<std::unique_ptr<Node>> m_nodes;
		std::vector<std::unique_ptr<Request>> m_requests;
	};
}