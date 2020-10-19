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
#include <string>

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
	killed = 0;

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

	npcs.clear();
	holes.clear();

	objectPositions.clear();

	Vector2 playerPos = { 0,0 };

	for (float row = 1; row < (m_windowHeight / 32) - 1; row++)
	{
		for (float column = 1; column < (m_windowWidth / 32); column++)
		{
			int x = (rand() % 100);

			Vector2 pos = {(column * 32) + 32 / 2, (row * 32) + 32 / 2 };

			if (x == 1)
			{
				GameObject* newObj = new GameObject({ pos, hole });
				objects.push_back(newObj);
				holes.push_back(newObj);
			}
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
	player->boo = boo;

	Vector2 hunterPos = { (float)m_windowWidth / 2, (float)m_windowHeight / 2 };

	hunterNPC->SetPosition(hunterPos);
	hunterNPC->SetBehaviour(hunterNPC->GetWanderBehaviour());
	hunterNPC->GetWanderBehaviour()->SetTarget(playerPos);

	npcs.push_back(hunterNPC); // Hunter
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

void Application::EndGame()
{
	gameOver = true;

	//for (GameObject* obj : objects)
	//{
	//	obj = nullptr;
	//	delete obj;
	//}

	//objects.clear();

	//m_graphEditor->DeleteGraph();

	//Map();
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
	human = GetTextureFromImage(LoadImage("human.png")); // 4
	boo = GetTextureFromImage(LoadImage("boo.png")); // 5
	hunter = GetTextureFromImage(LoadImage("hunter.png")); // 6

	hunterNPC = new NPC({ Vector2({ (float)m_windowWidth / 2, (float)m_windowHeight / 2 }), hunter });

	Map();
}

void Application::Unload()
{
	delete m_graph;
	m_graph = nullptr;
}

void Application::Update(float deltaTime)
{
	if (gameOver)
	{
		if (IsMouseButtonPressed(1))
		{
			gameOver = false;
			player->booWait = 0;
		}
		else return;
	}
		
	m_graphEditor->Update(deltaTime);

	Vector2 plyPos = player->GetPosition();
	camera.target = plyPos;

	if (IsKeyPressed(KEY_SPACE) && player->booWait <= 0)
	{
		hunterNPC->Seek(plyPos);

		for (auto npc : npcs)
		{
			if (!npc->falling && npc != hunterNPC)
			{
				if (Vector2Distance(npc->GetPosition(), plyPos) < 80)
				{
					FleeBehaviour* fleeBehave = npc->GetFleeBehaviour();

					fleeBehave->Flee(npc->GetPosition(), plyPos);

					npc->SetBehaviour(fleeBehave);
					npc->isFleeing = true;
				}
			}
		}
	}

	if (hunterNPC->seeking && player->booWait > 0)
	{
		if (Vector2Distance(hunterNPC->GetPosition(), plyPos) < 30)
		{
			EndGame();
		}
		else if (Vector2Distance(hunterNPC->GetPosition(), plyPos) < 200)
		{
			hunterNPC->Seek(plyPos);
		}
	}


	for (auto npc : npcs)
	{
		if (npc->dead && !npc->seeking)
		{
			Vector2 randPos = Vector2({ (float)(rand() % 32 + (m_windowWidth - 64)), (float)(rand() % 32 + (m_windowHeight - 64)) });

			npc->SetPosition(randPos);
			npc->SetBehaviour(npc->GetWanderBehaviour());
			npc->GetWanderBehaviour()->SetTarget(npc->GetPosition());

			npc->dead = false;
			npc->falling = false;
			npc->isFleeing = false;

			continue;
		}

		if (!npc->falling && !npc->seeking)
		{
			for (auto hole : holes)
			{
				Vector2 holePos = hole->GetPosition();
				holePos.x -= 16;
				holePos.y -= 16;

				if (Vector2Distance(npc->GetPosition(), holePos) < 20)
				{
					npc->Fall();
					npc->SetPosition(holePos);
					npc->SetBehaviour(nullptr);

					killed++;
				}
			}

			if (!npc->falling && !npc->isFleeing && player->booWait <= 0 && !npc->seeking)
			{
				npc->SetBehaviour(npc->GetWanderBehaviour());
			}
		}

		Vector2 npcPos = npc->GetPosition();

		if ((npcPos.x < 0 || npcPos.x > m_windowWidth) || (npcPos.y < 0 || npcPos.y > m_windowHeight))
		{
			npcPos.x = m_windowWidth / 2;
			npcPos.y = m_windowHeight / 2;

			npc->SetBehaviour(npc->GetWanderBehaviour());
			npc->GetWanderBehaviour()->SetTarget(npcPos);
			npc->SetVelocity(Vector2({ 0,0 }));
		}

		npcPos.x = ((npcPos.x < 32) ? 32 : ((npcPos.x > m_windowWidth - 32) ? m_windowWidth - 32 : npcPos.x));
		npcPos.y = ((npcPos.y < 32) ? 32 : ((npcPos.y > m_windowHeight - 32) ? m_windowHeight - 32 : npcPos.y));

		npc->SetPosition(npcPos);
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

			DrawText("Ghosts Revenge", 10, -50, 50, DARKPURPLE);

		EndMode2D();

	std::string display = "Kills: " + std::to_string(killed);
	DrawText(display.c_str(), 10, 10, 50, BLACK);

	if (gameOver)
	{
		DrawCube({ (float)m_windowHeight, (float)m_windowHeight/2 }, m_windowWidth, m_windowHeight, 1, GRAY);
		DrawText("GAME OVER", 200, m_windowHeight / 2 - 200 / 2, 200, RED);
		DrawText("RIGHT CLICK TO RESTART", 200, m_windowHeight / 2 + 200 / 2, 60, WHITE);
	}

	EndDrawing();
}