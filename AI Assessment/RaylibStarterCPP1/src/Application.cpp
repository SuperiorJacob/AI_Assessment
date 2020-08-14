#include "Application.h"
#include "raylib.h"

#include "GameObject.h"
#include "Player.h"
#include "KeyboardBehaviour.h"
#include "SeekBehaviour.h"

#include "Graph.h"
#include "Graph2D.h"
#include "Graph2DEditor.h"

#include "NPC.h"

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

void Application::Map()
{
	// THIS IS TO BE CHANGED LATER
	for (int side = 1; side < 3; side++)
	{
		// Across
		for (float i = 0; i < m_windowWidth / 32; i++)
		{
			objects.push_back(new GameObject({ Vector2({ i * 32 + 16, (side == 2) ? (float)(m_windowHeight - 32 + 16) : 16 }), obstacle }));
		}

		// Down
		for (float i = 0; i < (m_windowHeight / 32) - 2; i++)
		{
			objects.push_back(new GameObject({ Vector2({ (side == 2) ? (float)(m_windowWidth - 32 + 16) : 16, (i * 32) + 32 + 16 }), obstacle }));
		}
	}

	for (auto npc : npcs)
	{
		delete npc;
	}
	npcs.clear();

	objectPositions.clear();

	Vector2 playerPos = { 0,0 };

	for (float row = 1; row < (m_windowHeight / 32) - 1; row++)
	{
		for (float column = 1; column < (m_windowWidth / 32); column++)
		{
			int x = (rand() % 100);

			Vector2 pos = {(column * 32) + 32 / 2, (row * 32) + 32 / 2 };

			if (x == 1) objects.push_back(new GameObject({ pos, hole }));
			else if (x >= 85)
			{
				GameObject* newObj = new GameObject({ pos, obstacle });
				objects.push_back(newObj);
						
				objectPositions[pos.y] = !objectPositions[pos.y].empty() ? objectPositions[pos.y] : std::map<int, GameObject*>();
				objectPositions[pos.y][pos.x] = newObj;
			}
			else
			{
				if ((column < (m_windowWidth / 32) - 1) && (row < (m_windowHeight / 32) - 1))
					m_graph->AddNode(pos);

				if (x == 2) npcs.push_back(new NPC({ pos, human }));
				if (playerPos.x == 0 && playerPos.y == 0) playerPos = pos;
			}
		}
	}

	// PLAYER
	player->SetPosition(playerPos);
	player->SetTexture(ghost, 32, 32);

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
			bool addEdge = CanEdgeNearObject(node, connectedNode);

			if (addEdge) m_graph->AddEdge(node, connectedNode, dist);
			//m_graph->AddEdge(connectedNode, node, dist);
		}

		if (node->connections.empty()) 
		{ 
			m_graph->DeleteNode(node);
		}
	}

	m_graphEditor->SetGraph(m_graph);
	player->standingNode = player->NodeBelow(m_graphEditor);
	player->SetBehaviour(nullptr);

	// Path Finding Test
	//m_graphEditor->SetPath(m_graphEditor->GetGraph()->PathFind(player->standingNode, m_graphEditor->GetGraph()->GetNodes().back()));
	//
}

bool operator ==(const Vector2& l, const Vector2& r)
{
	return ((l.x == r.x) && (l.y == r.y));
}

bool Application::CanEdgeNearObject(Graph2D::Node* node, Graph2D::Node* toNode)
{
	if (node != nullptr && toNode != nullptr)
	{
		// Searching with a box formation.
		Vector2 node_pos = node->data;

		Vector2 toNode_pos = toNode->data;

		Vector2 vecToTarget = Vector2Subtract(toNode_pos, node_pos);
			
		Vector2 yPosCheck = Vector2Add(node_pos, { 0, vecToTarget.y });
		Vector2 xPosCheck = Vector2Add(node_pos, { vecToTarget.x, 0 });

		GameObject* obj = objectPositions[xPosCheck.y][xPosCheck.x];
		GameObject* obj1 = objectPositions[yPosCheck.y][yPosCheck.x];

		if (obj == nullptr && obj1 == nullptr) return true;

		return false;

		/*for (auto obj : objects)
		{
			if (obj == player) continue;

			Vector2 obj_pos = obj->GetPosition();
			if (Vector2Distance(xPosCheck, obj_pos) < obj->width/2 || 
				Vector2Distance(yPosCheck, obj_pos) < obj->height/2)
			{
				return false;
			}
		}*/
	}

	return true;
}

void Application::Load()
{
	camera = { 0 };
	camera.target = { 0, 0 };
	camera.offset = { (float)m_windowWidth / 2, (float)m_windowHeight / 2 };
	camera.rotation = 0.0f;
	camera.zoom = 2.0f;

	m_graphEditor = new Graph2DEditor();

	m_graph = new Graph2D();

	// TEXTURES
	ghost = GetTextureFromImage(LoadImage("ghost.png")); // 1
	hole = GetTextureFromImage(LoadImage("hole.png")); // 2
	obstacle = GetTextureFromImage(LoadImage("obstacle.png")); // 3
	human = GetTextureFromImage(LoadImage("human.png"));

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

	Vector2 plyPos = player->GetPosition();
	camera.target = plyPos;

	for (auto npc : npcs)
	{
		if (Vector2Distance(npc->GetPosition(), plyPos) < 100)
		{
			FleeBehaviour* fleeBehave = npc->GetFleeBehaviour();
			
			fleeBehave->Flee(npc->GetPosition(), plyPos);

			npc->SetBehaviour(fleeBehave);
		}
	}

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

			m_graphEditor->PathFromNode(player->standingNode, camera);

			player->SeekPath(m_graphEditor->GetPath(), m_graphEditor);
		}

		for (auto obj : objects)
		{
			obj->Update(deltaTime);
		}

		for (auto npc : npcs)
		{
			npc->Update(deltaTime);
		}
	}
}

void Application::Draw()
{
	BeginDrawing();
	BeginMode2D(camera);
	//ClearBackground(BLACK);

	ClearBackground(Color({ 142, 142, 142 }));

	if (IsKeyDown(KEY_TAB))
		m_graphEditor->Draw();

	for (auto obj : objects)
	{
		obj->Draw();
	}

	for (auto npc : npcs)
	{
		npc->Draw();
	}

	EndMode2D();
	EndDrawing();
}