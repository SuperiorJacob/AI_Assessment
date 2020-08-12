#include "KeyboardBehaviour.h"
#include "GameObject.h"
#include "raylib.h"

KeyboardBehaviour::KeyboardBehaviour(float moveForce) :
	Behaviour(),
	m_moveForce(moveForce)
{
	m_upKey = KEY_W;
	m_downKey = KEY_S;
	m_leftKey = KEY_A;
	m_rightKey = KEY_D;
}

KeyboardBehaviour::KeyboardBehaviour(int upKey, int downKey, int leftKey, int rightKey, float moveForce) :
	Behaviour(),
	m_upKey(upKey),
	m_downKey(downKey),
	m_leftKey(leftKey),
	m_rightKey(rightKey),
	m_moveForce(moveForce)
{

}

KeyboardBehaviour::~KeyboardBehaviour()
{
}

void KeyboardBehaviour::Update(GameObject* obj, float deltaTime)
{
	if (IsKeyDown(m_upKey)) obj->ApplyForce({ 0, -m_moveForce });
	if (IsKeyDown(m_downKey)) obj->ApplyForce({ 0, m_moveForce });
	if (IsKeyDown(m_leftKey)) obj->ApplyForce({ -m_moveForce, 0 });
	if (IsKeyDown(m_rightKey)) obj->ApplyForce({ m_moveForce, 0 });
}