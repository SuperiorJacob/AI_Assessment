#include "Player.h"
#include "KeyboardBehaviour.h"
#include "SeekBehaviour.h"
#include <iostream>

Player::Player()
{
	m_kbBehaviour = new KeyboardBehaviour(100);
	m_seekBehaviour = new SeekBehaviour();

	m_seekBehaviour->SetTargetRadius(10);
	m_seekBehaviour->OnArrive([this](){
		SetBehaviour(m_kbBehaviour);
	});

	m_moveSpeed = 2.0f;

	//SetBehaviour(m_kbBehaviour);
}

Player::~Player()
{
	SetBehaviour(nullptr);

	delete m_kbBehaviour;
	delete m_seekBehaviour;
}

void Player::Update(float deltaTime)
{
	if (GetBehaviour() != nullptr)
		GameObject::Update(deltaTime * m_moveSpeed);
}

void Player::Draw()
{
	GameObject::Draw();
}

Graph2D::Node* Player::NodeBelow(Graph2DEditor* graph)
{
	bool tooClose = false;

	std::vector<Graph2D::Node*> nearbyNodes;
	graph->GetGraph()->GetNearbyNodes(GetPosition(), graph->GetNodeRadius(), nearbyNodes);

	Graph2D::Node* click = graph->GetGraph()->GetNodes().front();

	for (auto nearbyNode : nearbyNodes)
	{
		float dist = Vector2Distance(GetPosition(), nearbyNode->data);
		int radius = graph->GetNodeRadius();

		tooClose = (dist <= radius);

		if (tooClose && (click != nullptr ? (Vector2Distance(GetPosition(), click->data) > dist) : true))
			click = nearbyNode;
	}

	return click;
}

void Player::SeekPath(std::list<Graph2D::Node*> fpath, Graph2DEditor* graph)
{
	SetPath(fpath);

	m_graph = graph;

	standingNode = NodeBelow(graph);
	m_seekBehaviour->SetTarget(path.back()->data);
	SetBehaviour(m_seekBehaviour);

	m_seekBehaviour->OnArrive([this]() {
		if (!path.empty())
		{
			path.pop_back();

			if (!path.empty() && path.back() != nullptr)
			{
				standingNode = path.back();
				m_seekBehaviour->SetTarget(Vector2Add(path.back()->data, { -32 / 2, -32 / 2 }));
			}
		}
		else
		{
			for (auto node : m_graph->GetPath())
			{
				node->onto = nullptr;
			}

			SetBehaviour(nullptr);
		};

		SetVelocity({ 0,0 }); // Stops the player from flying super fast.
	});
}
