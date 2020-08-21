#include "Graph2D.h"
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <functional>

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
	if (node->node == find)
	{
		return true;
	}

	return false;
}

std::list<Graph2D::Node*> Graph2D::PathFind(Node* start, Node* find)
{
	PFNode* pf = new PFNode(start, nullptr, 0);

	// Sort for queue
	auto pfSort = [](PFNode* a, PFNode* b) {
		return a->score > b->score;
	};

	std::priority_queue<PFNode*, std::vector<PFNode*>, decltype(pfSort)> stack(pfSort); // Stack
	//

	// Requirements
	std::list<PFNode*> visited; // Past nodes.

	std::map<Node *, PFNode*> gNodeLookup; // Look for a past node, see if it exists, if so, instant access to PFNode.

	std::list<Graph2D::Node*> path; // The path to return.
	//

	stack.push(pf);
	gNodeLookup[start] = pf;

	while (!stack.empty())
	{
		PFNode* node = stack.top();
		stack.pop();
		visited.push_back(node);

		if (Process(node, find))
		{
			// Push to path.
			while (node != nullptr)
			{
				node->node->onto = node->parent != nullptr ? node->parent->node : node->node;

				path.push_back(node->node);

				PFNode* oldNode = node;

				node = node->parent != node ? node->parent : nullptr;

				delete oldNode;
			}

			visited.empty();
			gNodeLookup.empty();

			break;
		}

		for (auto edge : node->node->connections)
		{
			float score = edge.data + node->score;

			PFNode* childPfNode = gNodeLookup[edge.to]; // Contains(stack, edge.to);
			// if (childPfNode == nullptr)
			// 	childPfNode = Contains(visited, edge.to);

			if (childPfNode == nullptr)
			{
				stack.push(new PFNode(edge.to, node, score));
				gNodeLookup[edge.to] = stack.top();
			}
			else
			{
				//std::cout << "Node Score: " << childPfNode->score << '\t' << "Optimal Score:" << score << std::endl;
				if (childPfNode->score > score)
				{
					//std::cout << "Best direction" << std::endl << std::endl;
					childPfNode->parent = node;
					childPfNode->score = score;
				}
			}
		}
	}

	return path;
}
