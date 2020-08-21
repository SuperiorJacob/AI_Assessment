#include "WanderBehaviour.h"
#include "GameObject.h"

#include <iostream>

WanderBehaviour::WanderBehaviour() : SeekBehaviour()
{
}

WanderBehaviour::~WanderBehaviour()
{
}

void WanderBehaviour::Update(GameObject* obj, float deltaTime)
{
	float distToTarget = Vector2Distance(obj->GetPosition(), m_target);
	if (distToTarget < m_targetRadius)
	{
		if (m_onArriveFn)
				m_onArriveFn();

		//obj->SetVelocity(Vector2({ 0,0 }));

		Vector2 newPos = obj->GetPosition();
		newPos.x += rand() % ((int)m_wanderRadius * 2) - m_wanderRadius;
		newPos.y += rand() % ((int)m_wanderRadius * 2) - m_wanderRadius;

		Wander(newPos);
	}

	Vector2 heading = Vector2Add(obj->GetPosition(), obj->GetVelocity());
	float headingLen = Vector2Length(heading);

	Vector2 dirToTarget = Vector2Normalize(Vector2Subtract(m_target, obj->GetPosition()));
	Vector2 vecToTarget = Vector2Scale(dirToTarget, headingLen * seekSpeed);

	Vector2 targetForcePos = Vector2Add(vecToTarget, obj->GetPosition());
	Vector2 forceDir = Vector2Subtract(targetForcePos, heading);

	obj->ApplyForce(forceDir);
}

void WanderBehaviour::Wander(Vector2 pos)
{
	SetTarget(pos);
}