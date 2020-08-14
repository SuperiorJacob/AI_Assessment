#pragma once
#include "SeekBehaviour.h"

class FleeBehaviour : public SeekBehaviour
{
public:

	FleeBehaviour();
	virtual ~FleeBehaviour();
	
	void Flee(Vector2 pos, Vector2 runFrom);
	void Update(GameObject* obj, float deltaTime);

	float seekSpeed = 1.0f;

private:
};