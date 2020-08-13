#include "Application.h"
#include "raylib.h"

#include "GameObject.h"
#include "Player.h"
#include "KeyboardBehaviour.h"
#include "SeekBehaviour.h"

#include "Graph.h"
#include "Graph2D.h"
#include "Graph2DEditor.h"

#include <iostream>

Application::Application(int windowWidth, int windowHeight, const char* windowTitle) :
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_windowTitle(windowTitle)
{

}

Application::~Application()
{
	for (auto obj : objects)
	{
		delete obj;
	}
}

void Application::Run()
{
	InitWindow(m_windowWidth, m_windowHeight, m_windowTitle);

	SetTargetFPS(60);

    Load();

    while (!WindowShouldClose())
    {
		float deltaTime = GetFrameTime();
		Update(deltaTime);
		Draw();
    }

    Unload();

	CloseWindow();
}

Texture2D GetTextureFromImage(Image img)
{
	Texture2D texture = LoadTextureFromImage(img);

	UnloadImage(img);

	return texture;
}

Player ply;
void Application::Map()
{
	// THIS IS TO BE CHANGED LATER
	for (int side = 1; side < 3; side++)
	{
		for (float i = 0; i < m_windowWidth / 32; i++)
		{
			objects.push_back(new GameObject({ Vector2({ i * 32, (side == 2) ? (float)(m_windowHeight - 32) : 0 }), obstacle }));
		}

		for (float i = 0; i < (m_windowHeight / 32) - 2; i++)
		{
			objects.push_back(new GameObject({ Vector2({ (side == 2) ? (float)(m_windowWidth - 32) : 0, (i * 32) + 32 }), obstacle }));
		}
	}

	Vector2 playerPos = { 0,0 };

	for (float row = 1; row < (m_windowHeight / 32) - 1; row++)
	{
		for (float column = 1; column < (m_windowWidth / 32); column++)
		{
			int x = (rand() % 100);

			if (x == 1) objects.push_back(new GameObject({ Vector2({ (column * 32), (row * 32) }), hole }));
			else if (x >= 85) objects.push_back(new GameObject({ Vector2({ (column * 32), (row * 32) }), obstacle }));
			else
			{
				if ((column < (m_windowWidth / 32) - 1) && (row < (m_windowHeight / 32) - 1))
					m_graph->AddNode({
						(column * 32) + 32 / 2, (row * 32) + 32 / 2 });

				if (playerPos.x == 0 && playerPos.y == 0) playerPos = { (column * 32), (row * 32) };
			}
		}
	}

	// PLAYER
	player->SetPosition(playerPos);
	player->SetTexture(ghost);

	objects.push_back(player); // Player
	//

	for (auto node : m_graph->GetNodes())
	{
		std::vector<Graph2D::Node*> nearbyNodes;
		m_graph->GetNearbyNodes(node->data, m_graphEditor->GetNodeRadius(), nearbyNodes);

		for (auto connectedNode : nearbyNodes)
		{
			if (connectedNode == node)
				continue;

			float dist = Vector2Distance(node->data, connectedNode->data);
			bool addEdge = true;

			//Vector2 check = Vector2Add(node->data, { -32, 32 });
			//if (check.x == connectedNode->data.x && check.y == connectedNode->data.y)
			//	for (auto obj : objects)
			//	{
			//		if (obj != player)
			//		{
			//			float objDist = Vector2Distance(obj->GetPosition(), node->data);
			//			float obj1Dist = Vector2Distance(obj->GetPosition(), connectedNode->data);

			//			if (objDist < 64 && obj1Dist < 64)
			//				addEdge = false;
			//		}
			//	}

			if (addEdge) m_graph->AddEdge(node, connectedNode, dist);
			//m_graph->AddEdge(connectedNode, node, dist);
		}
	}

	m_graphEditor->SetGraph(m_graph);
	player->standingNode = player->NodeBelow(m_graphEditor);
	player->SetBehaviour(nullptr);

	// Path Finding Test
	//m_graphEditor->SetPath(m_graphEditor->GetGraph()->PathFind(player->standingNode, m_graphEditor->GetGraph()->GetNodes().back()));
	//
}

void Application::Load()
{
	m_graphEditor = new Graph2DEditor();

	m_graph = new Graph2D();

	// TEXTURES
	ghost = GetTextureFromImage(LoadImage("ghost.png")); // 1
	hole = GetTextureFromImage(LoadImage("hole.png")); // 2
	obstacle = GetTextureFromImage(LoadImage("obstacle.png")); // 3

	Map();
}

void Application::Unload()
{
	delete m_graph;
	m_graph = nullptr;
}

void Application::Update(float deltaTime)
{
	m_graphEditor->Update(deltaTime);

	if (IsMouseButtonPressed(1))
	{
		for (GameObject * obj : objects)
		{
			obj = nullptr;
			delete obj;
		}

		objects.clear();

		m_graphEditor->DeleteGraph();

		Map();
	}
	else
	{
		if (IsMouseButtonPressed(0))
		{
			player->standingNode = player->NodeBelow(m_graphEditor);

			m_graphEditor->PathFromNode(player->standingNode);

			player->SeekPath(m_graphEditor->GetPath(), m_graphEditor);
		}

		for (auto obj : objects)
		{
			obj->Update(deltaTime);
		}
	}
}

void Application::Draw()
{
	BeginDrawing();
	//ClearBackground(BLACK);

	ClearBackground(Color({ 142, 142, 142 }));

	m_graphEditor->Draw();

	for (auto obj : objects)
	{
		obj->Draw();
	}

	EndDrawing();
}