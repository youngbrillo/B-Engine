#pragma once
#ifndef GAME_H
#define GAME_H


#include "Camera.h"
#include "Shader.h"
#include "Surface.h"


#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "PhysDebug.h"


class Game
{
public:
	Game();
	virtual ~Game();

	void DrawTitle(const char* string);
	virtual void Create() {};
	virtual void FixedUpdate(float deltaTime);
	virtual void Update(float deltaTime);
	virtual void DrawScene();
	virtual void DrawForeGround() {};
	virtual void DrawBackGround() {};
	virtual void DrawDebug();
	virtual void processInput(GLFWwindow *window, float deltaTime);

	virtual void Keyboard(int key) {};
	virtual void KeyboardUp(int key) {};
	virtual void KeyboardDown(int key) {};

	virtual void onBeginContact(b2Contact* contact);
	virtual void onEndContact(b2Contact* contact);
	virtual void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) {}
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) {}
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) {}
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) {}

public: //things shared by every game that inherits from this class
	static b2World* m_World;
	static Camera* AppCam;
	static GLFWwindow* mainWindow; 
	static glm::vec4 windowColorBackGround;
protected:
	b2Body* m_groundBody;
	Shader* m_Shader;
	Surface* m_surface; //billboards aka quads
	
	PhysicsDebugger* m_physDrawer;

};

typedef Game* Game_CreateFunction();
int RegisterGame(const char* category, const char* name, Game_CreateFunction* func);

struct GameEntry
{
	const char* category;
	const char* name;
	Game_CreateFunction* creationFunc;
};

#define MAX_APPS 256

extern GameEntry g_GameEntries[MAX_APPS];
extern int g_GameCount;

#endif // !GAME_H
