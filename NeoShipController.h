#pragma once
#include "Ship.h"
#include "OmniLauncher.h"
#include "CameraController.h"
#include "NeoInventory.h"
#include "ShipControllerUI.h"
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
	Ship* GetShip() { return m_ship; }
private: //private variables
	Ship* m_ship;
	OmniLauncher* m_launcher;
	CameraController* m_camera_controller;
	bool mouseAiming, shooting;
	b2Vec2 projectileLocalLaunchPos, projectileLaunchVel;
	ShipControllerUI* m_UI;

	friend class ShipControllerUI;
	friend class NeoInventory;

public:
	b2Vec2 target;
	glm::vec2 mouse_position;
	float launchVelocity;
	NeoInventory m_inventory;

	void UpdateMouse();

	bool condactIterationDone;

};

