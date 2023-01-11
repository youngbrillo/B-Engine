#pragma once
#ifndef SHIPCONTROLLER_H
#define SHIPCONTROLLER_H

#include "Spaceship.h"
#include "Bullet.h"
// ************* DO NOT USE THE NAMESPACE IN THE HEADER FILE!!!!! ,it will introduce errors :( *******************
//using namespace SpaceBuster; 

#include "Inventory.h"
#include "UI_Meter.h"


class ShipController
{
public:
	ShipController(b2World* worldreference, const b2Vec2& initalShipPos, b2Body* anchor = NULL, int levelDifficulty = 3);
	~ShipController();

	void FixedUpdate(float deltaTime);
	void Update(float deltaTime);

	void LaunchShip(b2World* worldreference, const b2Vec2& initalShip, b2Body* anchor = NULL);

	void handleBeginContact(b2Contact* contact);
	void handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	void handleEndContact(b2Contact* contact);

	void onKeyPress(int key);
	void onKeyRelease(int key);
	void onKeyRepeat(int key);

	void Draw();
	void DrawUI(Shader* customShader = nullptr, Surface* customSurface = nullptr);
	void Debug();

	
	bool AddToInventory(SpaceBuster::Item*);
	bool removeFromInventory(SpaceBuster::Item*);

	b2Body* getBody();

	Spaceship* m_ship;
	SpaceBuster::AutoLauncher* m_launcher;
	b2Vec2 initialSpawnPoint;
	int shipCount;
	b2Body* anchorRef;
	bool autoRespawn;
	static ShipController* g_shipController;
private:
	float respawnTimerCurrent, respawnTimerTarget;
public:
	SpaceBuster::Inventory* m_inventory;


	UIMeter conditionMeter, energyMeter, ammoMeter;
};

#endif // !SHIPCONTROLLER_H
