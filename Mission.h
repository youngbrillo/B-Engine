#pragma once
#ifndef MISSION_H
#define MISSION_H

#include <Box2D/Box2D.h>
#include <iostream>
#include "ObjectListener.h"

enum MissionState {
	ms_Active = 0,
	ms_Success = 1,
	ms_Failed  = 2
};
enum MissionType
{
	mt_ClearField = 0,
	mt_Delivery = 1,
	mt_Security = 2,
	mt_Excavation = 3,
	mt_BombDiffuse = 4
};
struct MissionDefinition
{
	//universal members
	std::string title = "[Insert Title Here]";
	std::string desc = "[Insert Description Here]";
	bool hasTimeLimit = true;
	float defaultTimeLimit = 120;

	b2Vec2 startPos = b2Vec2_zero;
	b2Vec2 endPos = b2Vec2(50.0f, 0.0f);

	b2Body* playerBody = nullptr;
	b2Body* targetBody = nullptr;

	void* reward;
	MissionType type = MissionType::mt_ClearField;
	int ClearRequirements = 100; //use this for the excavation and diffuse too 
	int DeliverStops = 1;
	

	bool contrainObjectsToScreen = true;
};


class Mission : public ObjectListener
{
public:
	Mission(MissionDefinition* definition);
	virtual ~Mission();
	//updates
	virtual void FixedUpdate(float deltaTime) {}
	virtual void Update(float deltaTime);

	//phys contact colliders
	virtual void onBeginContact(b2Contact* contact) {};
	virtual void onEndContact(b2Contact* contact) {};
	virtual void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) {};
	virtual void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {};


	virtual void onEnd() = 0; //the mission has to end, so when it ends, whats it do?
	virtual void onTimeLimitEnd();

	//display
	virtual void Debug();
	virtual void DrawUI();
	virtual void DrawContent() {};

	bool queryMissionEnd() { return hasEnded; }
	MissionState queryMissionStatus() { return state; }

	void setReturnedEarly(bool val);// { returnedEarly = val; }
public:
	std::string name, description;
	unsigned int id;
protected:
	bool hasTimeLimit, hasEnded, returnedEarly;
	MissionState state;
	float timeElapsed, timeLimit;
	

public:
	virtual void ObjectCreated(GameObject* G) = 0;
	virtual void ObjectDeleted(GameObject* G) = 0;
};
#endif // !MISSION_H