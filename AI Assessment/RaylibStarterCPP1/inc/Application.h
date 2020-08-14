#pragma once

#include <raymath.h>
#include "Player.h"
#include "NPC.h"

// Forward Declare
class GameObject;
class Graph2D;
class Graph2DEditor;

#include <list>
#include <map>

class Application 
{
public:

	Application(int windowWidth, int windowHeight, const char *windowTitle);
	~Application();

	void Run();

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Draw();

	void Map();

	bool CanEdgeNearObject(Graph2D::Node* node, Graph2D::Node* toNode);

protected:
private:

	int m_windowWidth;
	int m_windowHeight;
	const char* m_windowTitle;

	//GameObject* m_player1 = nullptr;
	Graph2DEditor* m_graphEditor = nullptr;
	Graph2D* m_graph = nullptr;

	Texture2D ghost;
	Texture2D obstacle;
	Texture2D hole;
	Texture2D human;

	Player* player = new Player();

	std::list<GameObject*> objects;
	std::list<NPC*> npcs;

	std::map<int, std::map<int, GameObject*>> objectPositions;

	Camera2D camera;
};