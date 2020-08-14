#include "FleeBehaviour.h"
#include "GameObject.h"

#include <iostream>
FleeBehaviour::FleeBehaviour() : SeekBehaviour()
{
}

FleeBehaviour::~FleeBehaviour()
{
}

void FleeBehaviour::Update(GameObject* obj, float deltaTime)
{
	float distToTarget = Vector2Distance(obj->GetPosition(), m_target);
	if (distToTarget < m_targetRadius)
	{
		if (m_onArriveFn)
			m_onArriveFn();
	}

	Vector2 heading = Vector2Add(obj->GetPosition(), obj->GetVelocity());
	float headingLen = Vector2Length(heading);

	Vector2 dirToTarget = Vector2Normalize(Vector2Subtract(m_target, obj->GetPosition()));
	Vector2 vecToTarget = Vector2Scale(dirToTarget, headingLen * seekSpeed);

	Vector2 targetForcePos = Vector2Add(vecToTarget, obj->GetPosition());
	Vector2 forceDir = Vector2Subtract(targetForcePos, heading);

	obj->ApplyForce(forceDir);
}

void FleeBehaviour::Flee(Vector2 pos, Vector2 runFrom)
{
	Vector2 runAway = Vector2Normalize(Vector2Subtract(runFrom, pos));
	SetTarget(Vector2Scale(runAway, 5));
}