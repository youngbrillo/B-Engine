#pragma once
#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "Camera.h"
#include <Box2D/Box2D.h>
#include "Shader.h" 
#include "Surface.h"
#include "Transform.h"
//I COULD make a generic controller for cameras... but i'll just make this one custom ^_^
class CameraController
{
public:
	enum FollowMode {
		Instant = 0, //camera moves instantly to target position
		Lerp = 1, //camera lerps to target position, makes an animation
		Bound = 2	 //camera awakes and lerps to target position, when target goes out of bounds, then sleeps when reaching the target

	};
public:
	CameraController() {};
	CameraController(Camera* targetCam, const b2Body* targetBody, glm::vec2 offset = glm::vec2(0.0f, 0.0f), glm::vec2 limits = glm::vec2(5.0f, 5.0f));
	~CameraController();

	void Update(float dt);
protected:
	bool onTarget();
	void Follow();
	void FollowLerp(float dt);
	void FollowBound(float dt);
public:
	void handleBeginContact(b2Contact* contact);
	void handleEndContact(b2Contact* contact);
	void handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold);

public:
	void Draw(Shader* shader, Surface* surface);
	bool DrawDebug;
	Transform transform;
	void Debug();
private:
	Camera* cam;
	const b2Body* target;

	glm::vec2 cameraOffset, boundLimits;
	float followSpeed;
	bool enabled, sleep;

public:
	FollowMode mode;
	bool getEnabled() { return enabled; }
	void setEnabled(bool e) { enabled = e; }
	float rec_followSpeed = 0.0f;
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.3f);
	glm::vec4 colorbase = glm::vec4(1.0f, 1.0f, 1.0f, 0.3f);
	glm::vec4 colorlinear = glm::vec4(0.0f, 0, 1.0, 0.3f);
	glm::vec4 colorfollow = glm::vec4(0, 1.0f, 0, 0.3f);
	glm::vec4 colorOutofbounds = glm::vec4(1.0f, 0.0f, 0.0f, 0.3f);
	glm::vec2 TargetLastPosition;

private:
	void moveToTargetAtSpeed(float deltaTime, const glm::vec2& direction);
	
};
#endif // !CAMERACONTROLLER_H

