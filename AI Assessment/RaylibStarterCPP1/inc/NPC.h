#pragma once

#include "raymath.h"
#include <list>
#include "Graph2D.h"
#include "Player.h"
#include "FleeBehaviour.h"
#include "WanderBehaviour.h"

class NPC : public Player
{
public:

	NPC(Vector2 pos, Texture2D texture) 
	{
		GameObject::SetPosition(pos);
		GameObject::SetTexture(texture, texture.width, texture.height);

		m_seekBehaviour = new SeekBehaviour();
		m_wanderBehaviour = new WanderBehaviour();
		m_fleeBehaviour = new FleeBehaviour();

		m_fleeBehaviour->SetTargetRadius(100);
		m_fleeBehaviour->OnArrive([this]() {
			SetBehaviour(nullptr);
			SetVelocity(Vector2({ 0,0 }));
			isFleeing = false;
			});

		m_wanderBehaviour->SetTarget(GetPosition());

		m_moveSpeed = 150.0f;
	};

	virtual ~NPC();

	virtual void Update(float deltaTime);
	virtual void Draw();

	void Fall();
	void Seek(Vector2 pos);

	bool falling = false;
	bool isFleeing = false;
	bool dead = false;
	bool seeking = false;

	FleeBehaviour* GetFleeBehaviour();
	WanderBehaviour* GetWanderBehaviour();

protected:
	FleeBehaviour* m_fleeBehaviour;
	WanderBehaviour* m_wanderBehaviour;

	float fall = 0;

private:
};