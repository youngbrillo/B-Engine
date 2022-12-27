#pragma once
#ifndef APP_H
#define APP_H


#include "CSApp.h"
#include "Game.h"

#include <stdio.h>
#include <algorithm>
#include <vector>
//#include "Camera.h"
class App :  public CSApp
{
public:
	App(unsigned int width, unsigned int height);
	~App();

	virtual void Init();

	virtual void ProcessInput(GLFWwindow*, float deltaTime);
	virtual void FixedUpdate(float deltaTime);
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void KeyCallBack(GLFWwindow* win, int key, int scancode, int action, int mode);
	virtual void mouseCallback(GLFWwindow*, int button, int action, int mode);
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y);
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height);

protected:
	virtual void DebugRender();
	void RenderDifferentApp();
	void restartGame();

	void orderGameEntries();

	Game*	m_game;
	int		m_gameSelection;
	Camera* m_Camera;

};


#endif // !APP_H
