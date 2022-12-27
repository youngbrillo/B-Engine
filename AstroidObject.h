#pragma once
#ifndef ASTROIDOBJECT_H
#define ASTROIDOBJECT_H

#include "GameObject.h"
#include "Utility_Physics.h"


enum size_class {
	insignifigant = 0,
	cautionary = 1,
	imposing = 2,
};


typedef struct AstroidState
{
	b2Vec2	position		= b2Vec2_zero, 
			size			= b2Vec2_zero,
			linearVelocity	= b2Vec2_zero;
	float angularVelocity	= 0.0f;
};

class AstroidObject : public GameObject
{
public:
	//do i want the astroid to be responsible for it's own random definition? no
	AstroidObject(GameObjectDefinition* def, void* Creator = nullptr);
	~AstroidObject();

	void Update(float dt) override;

	void handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
	void Debug() override;
protected:
private:
	float health, expireTimeOut, expireTimeLeft;
	collisionType_astroids collisionType;
public:
	bool broken, fragmentFlag;
	size_class weightClass;
	void* m_Creator;
};

#endif // !ASTROIDOBJECT_H