#pragma once
#include "raylib.h"
#include "raymath.h"
#include <list>
#include "Graph2D.h"

class Behaviour;

class GameObject
{
public:

	GameObject();
	GameObject(Vector2 pos, Texture2D texture) : m_position(pos), m_texture(texture) { };

	virtual ~GameObject();

	virtual void Update(float deltaTime);
	virtual void Draw();

	void ApplyForce(const Vector2& force);

	// Getters
	const Vector2& GetPosition() const;
	const Vector2& GetVelocity() const;
	const float& GetFriction() const;

	Behaviour* GetBehaviour();

	// Setters
	void SetPosition(const Vector2& pos);
	void SetVelocity(const Vector2& vel);
	void SetFriction(const float& friction);

	// Path finding if used
	void SetPath(std::list<Graph2D::Node*> nPath) { path = nPath; };

	std::list<Graph2D::Node*> GetPath() { return path; };
	//

	Texture2D GetTexture() { return m_texture; };
	void SetTexture(Texture2D texture, int w, int h) { m_texture = texture; width = w; height = h; };

	void SetBehaviour(Behaviour* behaviour);

	int width = 32;
	int height = 32;

	bool debug = false;

protected:

	Texture2D m_texture;

	Vector2 m_position = { 0,0 };
	Vector2 m_velocity = { 0,0 };
	Vector2 m_acceleration = { 0,0 };
	float m_friction = 0.0f;
	float m_moveSpeed = 1.0f;

	Behaviour* m_behaviour = nullptr;

	std::list<Graph2D::Node*> path;

private:
};