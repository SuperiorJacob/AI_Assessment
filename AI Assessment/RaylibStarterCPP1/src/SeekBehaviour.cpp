#include "SeekBehaviour.h"
#include "GameObject.h"

SeekBehaviour::SeekBehaviour() : Behaviour()
{

}

SeekBehaviour::~SeekBehaviour()
{
}

void SeekBehaviour::Update(GameObject* obj, float deltaTime)
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
	Vector2 vecToTarget = Vector2Scale(dirToTarget, headingLen);

	Vector2 targetForcePos = Vector2Add(vecToTarget, obj->GetPosition());
	Vector2 forceDir = Vector2Subtract(targetForcePos, heading);

	obj->ApplyForce(forceDir);
}

void SeekBehaviour::Draw(GameObject* obj)
{
	DrawCircle(m_target.x, m_target.y, m_targetRadius, LIGHTGRAY);
	DrawCircle(m_target.x, m_target.y, 4, GRAY);
}

const Vector2& SeekBehaviour::GetTarget() const
{
	return m_target;
}

void SeekBehaviour::SetTarget(const Vector2& target)
{
	m_target = target;
}

const float& SeekBehaviour::GetTargetRadius() const
{
	return m_targetRadius;
}

void SeekBehaviour::SetTargetRadius(const float& radius)
{
	m_targetRadius = radius;
}

void SeekBehaviour::OnArrive(std::function<void()> callback)
{
	m_onArriveFn = callback;
}
