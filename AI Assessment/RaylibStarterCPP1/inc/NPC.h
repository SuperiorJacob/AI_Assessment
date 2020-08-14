#pragma once

#include "raymath.h"
#include <list>
#include "Graph2D.h"
#include "Player.h"
#include "FleeBehaviour.h"

class NPC : public Player
{
public:

	NPC(Vector2 pos, Texture2D texture) 
	{
		GameObject::SetPosition(pos);
		GameObject::SetTexture(texture, texture.width, texture.height);

		m_seekBehaviour = new SeekBehaviour();
		m_fleeBehaviour = new FleeBehaviour();

		m_seekBehaviour->SetTargetRadius(10);
		m_seekBehaviour->OnArrive([this]() {
			SetBehaviour(nullptr);
			});

		m_moveSpeed = 150.0f;
	};

	virtual ~NPC();

	virtual void Update(float deltaTime);
	virtual void Draw();

	FleeBehaviour* GetFleeBehaviour();

protected:
	FleeBehaviour* m_fleeBehaviour;
private:
};