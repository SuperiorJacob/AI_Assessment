#include "NPC.h"
#include <iostream>

NPC::~NPC()
{
	delete m_fleeBehaviour;
	//Player::~Player();
}

void NPC::Update(float deltaTime)
{
	if (GetBehaviour() != nullptr)
		GameObject::Update(deltaTime);
}

void NPC::Draw()
{
	//GameObject::Draw();
	if (m_behaviour != nullptr) m_behaviour->Draw(this);
	Vector2 pos = GetPosition();

	if (falling && fall > 0) 
	{
		fall = fall - (float)0.01;
		float scale = 16;
		pos.x += scale;
		pos.y += scale;

		DrawTextureEx(GetTexture(), pos, 0, fall, WHITE);
	}
	else if (falling && fall <= 0)
	{
		dead = true;
	}
	else if (!falling)
	{
		fall = 0;

		DrawTextureEx(GetTexture(), pos, 0, 1, WHITE);
	}
}

void NPC::Fall()
{
	fall = 1;
	falling = true;
}

void NPC::Seek(Vector2 pos)
{
	seeking = true;

	m_seekBehaviour->SetTarget(pos);
	m_seekBehaviour->SetTargetRadius(10);
	m_seekBehaviour->OnArrive([this]() {
		m_wanderBehaviour->SetTarget(GetPosition());
		SetBehaviour(m_wanderBehaviour);
		seeking = false;
		});

	SetBehaviour(m_seekBehaviour);
}

FleeBehaviour* NPC::GetFleeBehaviour()
{
	return m_fleeBehaviour;
}

WanderBehaviour* NPC::GetWanderBehaviour()
{
	return m_wanderBehaviour;
}
