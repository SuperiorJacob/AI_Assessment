#include "NPC.h"
#include <iostream>

NPC::~NPC()
{
	delete m_fleeBehaviour;
	Player::~Player();
}

void NPC::Update(float deltaTime)
{
	if (GetBehaviour() != nullptr)
		GameObject::Update(deltaTime);
}

void NPC::Draw()
{
	GameObject::Draw();
}

FleeBehaviour* NPC::GetFleeBehaviour()
{
	return m_fleeBehaviour;
}
