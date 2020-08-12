#include "Graph2DEditor.h"
#include "Graph2D.h"
#include <iostream>
Graph2DEditor::Graph2DEditor()
{

}

Graph2DEditor::~Graph2DEditor()
{

}

void Graph2DEditor::Update(float deltaTime)
{
	auto mousePos = GetMousePosition();

	std::vector<Graph2D::Node*> nearbyNodes;
	m_graph->GetNearbyNodes(mousePos, GetNodeRadius(), nearbyNodes);

	Graph2D::Node* click = nullptr;

	for (auto nearbyNode : nearbyNodes)
	{
		float dist = Vector2Distance(mousePos, nearbyNode->data);
		int radius = GetNodeRadius();

		tooClose = (dist <= radius);

		if (tooClose)
			click = nearbyNode;

			break;
	}

	if (IsMouseButtonPressed(0))
	{
		if (tooClose)
		{
			for (auto node : GetGraph()->GetNodes())
			{
				node->onto = nullptr;
			}

			Graph2D::PFNode* updateFollow = GetGraph()->PathFind(GetGraph()->GetNodes().front(), click);

			while (updateFollow != nullptr)
			{
				updateFollow->node->onto = updateFollow->parent != nullptr ? updateFollow->parent->node : updateFollow->node;
				updateFollow = updateFollow->parent;
			}

			delete updateFollow;
		}
	}
}

void Graph2DEditor::Draw()
{
	// Draw all connections
	for (auto node : m_graph->GetNodes())
	{
		for (auto connection : node->connections)
		{
			DrawLine(node->data.x, node->data.y, connection.to->data.x, connection.to->data.y, m_nodeLineColour);
		}
	}

	//// Draw a "preview" node where the mouse is (lines)
	//auto mousePos = GetMousePosition();

	//std::vector<Graph2D::Node*> nearbyNodes;
	//m_graph->GetNearbyNodes(mousePos, m_nodeRadius, nearbyNodes);

	//for (auto nearbyNode : nearbyNodes)
	//{
	//	DrawLine(mousePos.x, mousePos.y, nearbyNode->data.x, nearbyNode->data.y, !tooClose ? m_nodeLineColour : RED);
	//}

	// Draw all nodes
	for (auto node : m_graph->GetNodes())
	{
		DrawCircle(node->data.x, node->data.y, 8, m_nodeColour);
		DrawCircleLines(node->data.x, node->data.y, 8, m_nodeOutlineColour);
	}

	// Draw if found
	for (auto node : m_graph->GetNodes())
	{
		for (auto connection : node->connections)
		{
			if (node->onto == connection.to)
				DrawLine(node->data.x, node->data.y, connection.to->data.x, connection.to->data.y, m_nodeFoundColour);
		}
	}

	for (auto node : m_graph->GetNodes())
	{
		if (node->onto != nullptr)
		{
			DrawCircle(node->data.x, node->data.y, 8, m_nodeFoundColour);
			DrawCircleLines(node->data.x, node->data.y, 8, m_nodeFoundColour);
		}
	}

	//// Draw a "preview" node where the mouse is
	//DrawCircle(mousePos.x, mousePos.y, 8, m_nodeColour);
}

Graph2D* Graph2DEditor::GetGraph()
{
	return m_graph;
}

void Graph2DEditor::SetGraph(Graph2D* graph)
{
	m_graph = graph;
}

// Config
void Graph2DEditor::SetNodeRadius(int radius)
{
	m_nodeRadius = radius;
}

void Graph2DEditor::SetNodeColour(Color colour)
{
	m_nodeColour = colour;
}

void Graph2DEditor::SetNodeOutlineColour(Color colour)
{
	m_nodeOutlineColour = colour;
}

void Graph2DEditor::SetNodeLineColour(Color colour)
{
	m_nodeLineColour = colour;
}

int Graph2DEditor::GetNodeRadius()
{
	return m_nodeRadius;
}

Color Graph2DEditor::GetNodeColour()
{
	return m_nodeColour;
}

Color Graph2DEditor::GetNodeOutlineColour()
{
	return m_nodeOutlineColour;
}

Color Graph2DEditor::GetNodeLineColour()
{
	return m_nodeLineColour;
}

void Graph2DEditor::DeleteGraph()
{
	m_graph->DeleteNodes();
	m_graph = nullptr;
}
