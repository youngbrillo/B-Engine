#pragma once
#include "Mission.h"
#include <vector>
#include "ScoreManager.h"
#include <glm/glm.hpp>

class MissionSurvival :public Mission
{
public:
	MissionSurvival(MissionDefinition* def);
	virtual ~MissionSurvival();

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
	std::vector<GameObject*> localGameObjects;
	b2Vec2 spawnOffset;
	b2Body* exitBody;
	bool contrainObjectsToScreen;

private:
	void repositionOffscreenObjects();
	std::string getTimeLefts();
	float getTimeLeftf();

private:
	SpaceBuster::ScoreManager* scoreManager;
	glm::ivec2 uiPos;
};

