#pragma once
#include "SeekBehaviour.h"

class WanderBehaviour : public SeekBehaviour
{
public:

	WanderBehaviour();
	virtual ~WanderBehaviour();

	void Wander(Vector2 pos);
	void Update(GameObject* obj, float deltaTime);

	float seekSpeed = 1.0f;
	float m_targetRadius = 10;
	float m_wanderRadius = 35;

private:
};