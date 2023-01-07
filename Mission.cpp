#include "Mission.h"

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
{
}

Mission::~Mission()
{
}

void Mission::Update(float deltaTime)
{
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

