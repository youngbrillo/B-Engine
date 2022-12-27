#pragma once
#ifndef CSAPP_H
#define CSAPP_H


//#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class CSApp
{
public:
	enum App_State {
		state_paused = 0,
		state_running = 1
	};

	CSApp(unsigned int width, unsigned int height);
	virtual void Init() = 0;
	App_State state;
	bool         Keys[1024];
	unsigned int screenWidth, screenHeight;


	virtual void ProcessInput(GLFWwindow*, float deltaTime) = 0;
	virtual void FixedUpdate(float deltaTime) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void KeyCallBack(GLFWwindow* win, int key, int scancode, int action, int mode) = 0;
	virtual void mouseCallback(GLFWwindow*, int button, int action, int mode) = 0;
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) = 0;
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) = 0;
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) = 0;
	virtual void DebugMenu();
	void setKey(int key, bool value);

protected:
	bool ImGUI_Demo_visible, showUI;
	float timeSinceLastFrame;
	float timeSinceLastFrame_Target;


public:
	static GLFWwindow* mainWindow;
};

#endif // !CSAPP_H
