#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
//#include "CameraController.h"

class Camera
{
public:
	Camera(glm::vec4 viewPort);
	virtual ~Camera();


	glm::mat4 GetViewMatrix();
	glm::mat4 getProjectionMatrix();

	void mouseCallback(GLFWwindow* window, int button, int action, int mode);
	void mouseCallback_Cursor(GLFWwindow* w, double x, double y);
	void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void debug();
	void Reset();
	void updateCamVectors();
protected:
	glm::mat4 CalculateView();
	glm::mat4 CalculateProjection();

public:
	glm::vec2 convertScreenToWorld(glm::vec2& mousePos);
	int Width, Height; //width and height of screen
public:
	glm::vec3	position,
				right, up, front;
	float zoom;
protected:
	glm::vec2 screenCenter, mousePoint;
	float	  pitch, yaw, movementSpeed, mouseSensitivity
			, nearPlane, farPlane
			, nP_perspective, fP_perspective
			, nP_Ortho, fp_Ortho
		;

	bool	  readingInput, firstMouse, enabled, rightMouseDown
			, isStatic, isOrtho, isNDCOnly;

	glm::mat4 view, projection;
	

	friend class CameraController;
};
#endif // !CAMERA_H

