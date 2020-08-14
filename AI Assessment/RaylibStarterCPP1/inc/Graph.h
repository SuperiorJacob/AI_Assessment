#pragma once

#include <vector>
#include <algorithm>

template<class TNodeData, class TEdgeData>
class Graph
{
public:
	
	struct Node;
	struct Edge;

	struct Node
	{
		TNodeData data;
		std::vector<Edge> connections;
		Node* onto = nullptr;
	};

	struct Edge
	{
		Node* to;
		TEdgeData data;
	};

public:

	// ================
	// Constructor
	// ================
	Graph()
	{

	}

	// ================
	// Destructor
	// ================
	virtual ~Graph()
	{
		for (auto node : m_nodes)
		{
			delete node;
		}

		m_nodes.clear();
	}

	// ================
	// Add Node
	// ================
	Node* AddNode(const TNodeData& data)
	{
		Node* node = new Node();
		node->data = data;
		m_nodes.push_back(node);
		return node;
	}

	// ================
	// Add Edge
	// ================
	void AddEdge(Node* nodeA, Node* nodeB, const TEdgeData& data)
	{
		nodeA->connections.push_back({ nodeB, data });
	}

	// ================
	// Get Nodes
	// ================
	const std::vector<Node*>& GetNodes()
	{
		return m_nodes;
	}

	void DeleteNodes()
	{
		for (auto p : m_nodes) { delete p; }

		m_nodes.clear();
	}

	void DeleteNode(Node* node)
	{
		m_nodes.erase(std::remove(m_nodes.begin(), m_nodes.end(), node), m_nodes.end());
	}
protected:

	std::vector<Node*> m_nodes;

private:
};