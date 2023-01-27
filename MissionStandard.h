#pragma once
#ifndef MISSIONSTANDARD_H
#define MISSIONSTANDARD_H

#include "Mission.h"
#include <iostream>
#include <vector>
//this mission type requires the player to destroy a certain amount of astroids, 
//maybe, or maybe not under a time limit

class MissionStandard :  public Mission
{
public:
	MissionStandard(MissionDefinition* def);
	virtual ~MissionStandard();

	virtual void Update(float deltaTime)override;
	virtual void onBeginContact(b2Contact* contact) override;
	virtual void onEndContact(b2Contact* contact) override;

	virtual void onEnd();

	//virtual void onTimelimitEnd();

	virtual void Debug() override;
	virtual void DrawUI() override;
	virtual void DrawContent() override;


	virtual void ObjectCreated(GameObject* G);
	virtual void ObjectDeleted(GameObject* G);

private:
	int targetsEliminated, targetsGoal;
	std::string statusString;
	//std::vector<GameObject*> localGameObjects;
	//b2Vec2 spawnOffset;
	b2Body* exitBody;
//	bool contrainObjectsToScreen;

private:
	//void repositionOffscreenObjects();
	std::string getTimeLefts();
	float getTimeLeftf();
};

#endif // !MISSIONSTANDARD_H