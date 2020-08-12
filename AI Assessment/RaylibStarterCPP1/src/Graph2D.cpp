#include "Graph2D.h"
#include <iostream>

Graph2D::Graph2D() 
{

}

Graph2D::~Graph2D()
{
}

void Graph2D::GetNearbyNodes(Vector2 position, float radius, std::vector<Graph2D::Node*>& out_nodes)
{
	for (auto node : m_nodes)
	{
		float dist = Vector2Distance(position, node->data);
		if (dist < radius)
		{
			out_nodes.push_back(node);
		}
	}
}

bool Graph2D::Process(PFNode* node, Node* find)
{
	std::cout << node->score << std::endl;
	if (node->node == find)
	{
		std::cout << "FOUND NODE " << node->node->data.x << " " << node->node->data.y << std::endl;
		return true;
	}

	return false;
}

Graph2D::PFNode* Graph2D::Contains(std::vector<Graph2D::PFNode*> list, Graph2D::Node* findNode)
{
	for (auto node : list)
	{
		if (node->node == findNode)
			return node;
	}

	return nullptr;
}

Graph2D::PFNode* Graph2D::PathFind(Node* start, Node* find)
{
	PFNode* pf = new PFNode(start, nullptr, 0);

	std::vector<PFNode*> stack;
	std::vector<PFNode*> visited;

	stack.push_back(pf);

	while (!stack.empty())
	{
		PFNode* node = stack.back();
		stack.pop_back();
		visited.push_back(node);

		if (Process(node, find))
		{
			return node;
		}

		for (auto edge : node->node->connections)
		{
			float score = edge.data + node->score;

			PFNode* childPfNode = Contains(stack, edge.to);
			if (childPfNode == nullptr)
				childPfNode = Contains(visited, edge.to);

			if (childPfNode == nullptr)
			{
				// TODO: create a new PfNode and ad to the stack
				stack.push_back(new PFNode(edge.to, node, score));
			}
			else
			{
				if (childPfNode->score > score)
				{
					childPfNode->parent = node;
					childPfNode->score = score;
				}
			}
		}
	}

	return pf;
}
