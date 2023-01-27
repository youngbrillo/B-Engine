#include "Mission.h"
#include "Game.h"

Mission::Mission(MissionDefinition* def)
	: ObjectListener()
	, name(def->title)
	, description(def->desc)
	, state(MissionState::ms_Active)
	, hasEnded(false)
	, hasTimeLimit(def->hasTimeLimit)
	, timeElapsed(0.0f)
	, timeLimit(def->defaultTimeLimit)
	, id(0)
	, returnedEarly(false)
	, contrainObjectsToScreen(def->contrainObjectsToScreen)
	, spawnOffset(b2Vec2_zero)
{
}

Mission::~Mission()
{
	localGameObjects.clear();
}

void Mission::Update(float deltaTime)
{
	repositionOffscreenObjects();

	if (hasEnded || returnedEarly) return;
	if (state != MissionState::ms_Active) return;
	if (!hasTimeLimit) return;

	timeElapsed += deltaTime;
	if (timeElapsed >= timeLimit)
	{
		timeElapsed == timeLimit;
		onTimeLimitEnd();
	}
}

void Mission::onTimeLimitEnd()
{
	//default behavior
	state = MissionState::ms_Failed;
	this->onEnd();
}


#include <imgui.h>
void Mission::Debug()
{
	if(ImGui::TreeNode(name.c_str()))
	{
		ImGui::Text(description.c_str());
		ImGui::Checkbox("bind objects to screen", &contrainObjectsToScreen);
		ImGui::SliderFloat("elapsed time", &timeElapsed, 0, timeLimit);
		ImGui::SliderFloat("time Limit", &timeLimit, 0, 180.0f);

		ImGui::TreePop();
	}
}

void Mission::DrawUI()
{
}

void Mission::setReturnedEarly(bool val)
{
	returnedEarly = true;
	hasEnded = true;
	this->onEnd();
}

void Mission::repositionOffscreenObjects()
{
	if (!contrainObjectsToScreen) return;
	glm::vec2 top(0.0f), bottom(Game::AppCam->Width, 1.0 * Game::AppCam->Height), lefttop, rightbottom;
	lefttop = Game::AppCam->convertScreenToWorld(top);
	rightbottom = Game::AppCam->convertScreenToWorld(bottom);

	for (auto i : localGameObjects)
	{
		b2Vec2 oldPosition = i->GetBody()->GetPosition();
		b2Vec2 newPosition = oldPosition;
		bool changeNeeded = false;

		if (oldPosition.y > lefttop.y + spawnOffset.y) //too far up
		{
			newPosition.y = rightbottom.y;
			changeNeeded = true;
		}
		if (oldPosition.y < rightbottom.y - spawnOffset.y) //too far down
		{
			newPosition.y = lefttop.y;
			changeNeeded = true;
		}

		if (oldPosition.x < lefttop.x - spawnOffset.x) //too far left
		{
			newPosition.x = rightbottom.x;
			changeNeeded = true;
		}

		if (oldPosition.x > rightbottom.x + spawnOffset.x) //too far right
		{
			newPosition.x = lefttop.x;
			changeNeeded = true;
		}


		if (changeNeeded)
		{
			newPosition = newPosition;
			i->m_body->SetTransform(newPosition, i->m_body->GetAngle());
		}
	}
}

