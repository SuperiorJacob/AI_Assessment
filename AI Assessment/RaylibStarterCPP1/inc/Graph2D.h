#pragma once

#include "Graph.h"
#include "raymath.h"

class Graph2D : public Graph<Vector2, float>
{
public:

	Graph2D();
	virtual ~Graph2D();

	void GetNearbyNodes(Vector2 position, float radius, std::vector<Graph2D::Node*>& out_nodes);

	struct PFNode
	{
		Node* node = nullptr;
		PFNode* parent = nullptr;

		float score = 0;

		PFNode() {}
		PFNode(Node* gNode, PFNode* p, float s) : node(gNode), parent(p), score(s) { }

	};

	bool Process(PFNode* node, Node* find);

	Graph2D::PFNode* Contains(std::vector<Graph2D::PFNode*> list, Graph2D::Node* findNode);

	PFNode* PathFind(Node* start, Node* find);

private:
protected:
};