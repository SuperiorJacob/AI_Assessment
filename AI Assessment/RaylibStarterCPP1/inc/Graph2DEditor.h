#pragma once
#include <raymath.h>

class Graph2D;

class Graph2DEditor
{
public:
	
	Graph2DEditor();
	~Graph2DEditor();

	void Update(float deltaTime);
	void Draw();

	Graph2D* GetGraph();
	void SetGraph(Graph2D* graph);

	// Node Setup
	void SetNodeRadius(int radius);
	void SetNodeColour(Color colour);
	void SetNodeOutlineColour(Color colour);
	void SetNodeLineColour(Color colour);

	int GetNodeRadius();
	Color GetNodeColour();
	Color GetNodeOutlineColour();
	Color GetNodeLineColour();

	bool tooClose = false;

	void DeleteGraph();

protected:

	Graph2D* m_graph = nullptr;

	int m_nodeRadius = 60;
	Color m_nodeColour = PURPLE;
	Color m_nodeOutlineColour = DARKPURPLE;
	Color m_nodeLineColour = DARKPURPLE;
	Color m_nodeFoundColour = WHITE;

private:
};