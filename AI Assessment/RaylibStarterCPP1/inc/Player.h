#pragma once

#include "GameObject.h"
#include "Graph2D.h"
#include "Graph2DEditor.h"

class KeyboardBehaviour;
class SeekBehaviour;

class Player : public GameObject
{
public:

	Player();
	virtual ~Player();

	virtual void Update(float deltaTime);
	virtual void Draw();

	Graph2D::Node* standingNode = nullptr;

	Graph2D::Node* NodeBelow(Graph2DEditor* graph);

	SeekBehaviour* GetSeekBehaviour() { return m_seekBehaviour; };

	void SeekPath(std::list<Graph2D::Node*> path, Graph2DEditor* graph);

	Texture2D boo;
	int booWait = 0;

protected:

	KeyboardBehaviour* m_kbBehaviour;
	SeekBehaviour *m_seekBehaviour;

	Graph2DEditor* m_graph = nullptr;

	Vector2 booPos;

private:
};