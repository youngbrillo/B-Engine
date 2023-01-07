#pragma once
#ifndef SHIPCONTROLLER_H
#define SHIPCONTROLLER_H

#include "Spaceship.h"
#include "Bullet.h"
// ************* DO NOT USE THE NAMESPACE IN THE HEADER FILE!!!!! ,it will introduce errors :( *******************
//using namespace SpaceBuster; 

#include "Inventory.h"

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

	void Draw() {}
	void Debug();

	
	bool AddToInventory(SpaceBuster::Item*);
	bool removeFromInventory(SpaceBuster::Item*);

	b2Body* getBody();

	Spaceship* m_ship;
	SpaceBuster::Launcher* m_launcher;
	b2Vec2 initialSpawnPoint;
	int shipCount;
	b2Body* anchorRef;
	bool autoRespawn;
	static ShipController* g_shipController;
private:
	float respawnTimerCurrent, respawnTimerTarget;
public:
	SpaceBuster::Inventory* m_inventory;

};

#endif // !SHIPCONTROLLER_H
