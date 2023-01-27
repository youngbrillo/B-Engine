#pragma once
#include "Ship.h"
#include "OmniLauncher.h"
#include "CameraController.h"

struct NeoShipControllerDefinition
{
	//Ship params
	ShipAttributeDef shipDef;
	//Launcher params
	OmniLauncherDefinition launcherDef;
	b2Vec2 projectileLocalLaunchPos		= b2Vec2(0.0f, -0.5f);
	b2Vec2 projectileLaunchVel			= b2Vec2(0.0f, -50.0f);
};


class NeoShipController
{
public:
	NeoShipController(NeoShipControllerDefinition* controllerDefinition, b2Body* groundref = nullptr);
	~NeoShipController();
public: //frequent calls
	void FixedUpdate(float deltaTime);
	void Update(float deltaTime);
		//visual feedback
	void Draw(Shader* shader, Surface* surface);
	void Debug();
public: //physics
	void handleBeginContact(b2Contact* contact);
	void handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	void handleEndContact(b2Contact* contact);

public: //input handlers
	void mouseCallback(int button, int action, int mode);
	void mouseCallback_Cursor(double x, double y);

	void onKeyPress(int key);
	void onKeyRelease(int key);
	void onKeyRepeat(int key);

	void ShootProjectile();

private: //private variables
	Ship* m_ship;
	OmniLauncher* m_launcher;
	CameraController* m_camera_controller;
	bool mouseAiming, shooting;
	b2Vec2 projectileLocalLaunchPos, projectileLaunchVel;

public:
	b2Vec2 target;
	float launchVelocity;

};

