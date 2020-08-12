#pragma once

#include "Behaviour.h"
#include "raymath.h"

#include <functional>

class SeekBehaviour : public Behaviour
{
public:

	SeekBehaviour();
	virtual ~SeekBehaviour();

	virtual void Update(GameObject* obj, float deltaTime);
	virtual void Draw(GameObject* obj);

	const Vector2& GetTarget() const;
	void SetTarget(const Vector2& target);

	const float& GetTargetRadius() const;
	void SetTargetRadius(const float& radius);

	void OnArrive(std::function<void()> callback);

protected:

	Vector2 m_target = { 0,0 };
	float m_targetRadius = 1.0f;

	std::function<void()> m_onArriveFn;

private:
};