#pragma once

#include <raymath.h>

// Forward Declare
class GameObject;
class Graph2D;
class Graph2DEditor;

#include <list>

class Application 
{
public:

	Application(int windowWidth, int windowHeight, const char *windowTitle);
	~Application();

	void Run();

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Draw();

	void Map();

protected:
private:

	int m_windowWidth;
	int m_windowHeight;
	const char* m_windowTitle;

	//GameObject* m_player1 = nullptr;
	Graph2DEditor* m_graphEditor = nullptr;
	Graph2D* m_graph = nullptr;

	Texture2D ghost;
	Texture2D obstacle;
	Texture2D hole;

	struct Object
	{
		Vector2 pos;
		int type;
	};

	std::list<Object> objects;
};