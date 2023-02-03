#pragma once
#include <Box2D/Box2D.h>

#include "ShipAttributes.h"
#include "Utility_Physics.h"
#include "Shader.h"
#include "Surface.h"
class Ship
{
public:
	Ship(ShipAttributeDef* def, b2World* worldRef, const b2Vec2& spawnPos, b2Body* anchor = NULL);
	virtual ~Ship();

	virtual void FixedUpdate(float deltaTime);
	virtual void Update(float deltaTime);


	virtual void handleBeginContact(b2Contact* contact);
	virtual void handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	virtual void handleEndContact(b2Contact* contact);
	virtual void mouseCallback(int button, int action, int mode);
	virtual void mouseCallback_Cursor(double x, double y);

	virtual void onKeyPress(int key);
	virtual void onKeyRelease(int key);
	virtual void onKeyRepeat(int key);

	virtual void Draw();
	virtual void Draw(Shader* shader, Surface* surface);
	virtual void Debug();
	b2Body* getBody() { return mBody; }
	ShipAttributes* GetAttributes() { return &attributes; }
protected:
	void ApplyImpulseForce();
private:

	b2Body* mBody, *groundBodyRef;
	b2Joint* turnjointref;
	ShipAttributes attributes;
	float doubleTapIntervalLimit, doubleTapIntervalTimeLeft;
	bool doubleTappedForward, doubleTappedBackward, doubleTappedRight, doubleTappedLeft , listeningForDoubleTap, doubleTapEnabled , shifting;
	unsigned int lastKeyDown, lastKeyUp, doubleTapKey;

	float doubleTappVelocityMod;

	collisionType_astroids collisionType;

private:
	float targetAngle, rotationSpeed, maximum_speed, current_speed;
	b2Vec2 target;

	bool thrust_forward, thrust_backward, thrust_left, thrust_right, constrain_velocity;
	bool turn1 = false, turn2 = false, drawStats = true;
	friend class NeoShipController;
	friend class ShipControllerUI;

};

