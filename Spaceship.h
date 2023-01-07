#pragma once
#ifndef SPACESHIP_H
#define SPACESHIP_H
#include <Box2D/Box2D.h>
#include "Utility_Physics.h"
class Spaceship
{
public:
	Spaceship(b2World* worldreference, const b2Vec2& spawnPos, b2Body* anchor = NULL);
	~Spaceship();

	void FixedUpdate(float deltaTime);
	void Update(float deltaTime);


	void handleBeginContact(b2Contact* contact);
	void handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	void handleEndContact(b2Contact* contact);

	void onKeyPress(int key);
	void onKeyRelease(int key);
	void onKeyRepeat(int key);
	void Draw(){}
	void Debug();

	void modifyCondition(float value);
	void modifyEnergy(float value);

	b2Body* getBody() { return mainBody; };
	b2Fixture* fixture_leftside, * fixture_rightSide;

protected:
	b2Body* mainBody;
	b2Vec2  thrustForce, thrustPosition;
	float	torqueMag;

	bool thrust_forward, thrust_backward, thrust_left, thrust_right, operational, broken;

public:
	collisionType_astroids collisionType;

public:
	//if condition falls to 0, ship is destroyed

	//if energy falls to 0, ship can no longer move
	float condition, energy, maxCondition, maxEnergy;
	//physical defense to midigate deductions to condition from physical impacts
	//energy cost to gauge how quickly energy is drained via Thrusting
	//torqueRatio torque thrusts (to rotate the ship) don't cost as much as forward/backward thrusts, so it reprents how much we cut the energy cost by (ratio) when we turn
	float physicalDefense, energyCost, torqueRatio; 

private:
	friend class ShipController;
	//the limit is how long (from one touch to the next) until it can not be reasonably considered to be a double tap
	float doubleTapIntervalLimit, doubleTapIntervalTimeLeft;
	bool doubleTappedForward, doubleTappedBackward, doubleTappedRight, doubleTappedLeft;
	bool listeningForDoubleTap, doubleTapEnabled, shifting ;
	unsigned int lastKeyDown, lastKeyUp, doubleTapKey;
	float doubleTappVelocityMod, doubleTappTorqueMod, impulseCostModifier;

	void ApplyImpulseForce();
};



b2Body* createSimpleShipBody(b2World* World, const b2Vec2& spawnPos, Spaceship*, b2Body* ground = NULL);
b2Body* createArrowShipBody(b2World* World, const b2Vec2& spawnPos, Spaceship*, b2Body* ground = NULL);

#endif // !SPACESHIP_H
