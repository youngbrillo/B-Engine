#include "MissionSurvival.h"
#include "AstroidObject.h"
#include <imgui.h>

MissionSurvival::MissionSurvival(MissionDefinition* def)
	: Mission(def)
	, uiPos({10, 10})
	, elapsedTime(0.0f)
{
	scoreManager = new SpaceBuster::ScoreManager(2.0f, 10);
}

MissionSurvival::~MissionSurvival()
{
	delete scoreManager;

}

void MissionSurvival::Update(float deltaTime)
{
	elapsedTime += deltaTime;
	scoreManager->Update(deltaTime);
	Mission::Update(deltaTime);
}

void MissionSurvival::onBeginContact(b2Contact* contact)
{
}

void MissionSurvival::onEndContact(b2Contact* contact)
{
}

void MissionSurvival::onEnd()
{
}
void MissionSurvival::Debug()
{
	if(ImGui::TreeNode("- Mission: Survive -"))
	{
		ImGui::SliderInt2("UI Position", &uiPos.x, 0, 100);
		scoreManager->Debug();

		Mission::Debug();

		ImGui::TreePop();
	}

}

void MissionSurvival::DrawUI()
{
	scoreManager->Draw(uiPos.x, uiPos.y);
}

void MissionSurvival::DrawContent()
{
}

void MissionSurvival::ObjectCreated(GameObject* G)
{
	AstroidObject* someAstroid = dynamic_cast<AstroidObject*> (G);
	if (someAstroid)
	{
		localGameObjects.emplace_back(someAstroid);
	}
}

void MissionSurvival::ObjectDeleted(GameObject* G)
{
	AstroidObject* someAstroid = dynamic_cast<AstroidObject*> (G);
	if (!someAstroid) return;
	if (someAstroid->weightClass == size_class::imposing)
		scoreManager->IncrementScore(3);
	if (someAstroid->weightClass == size_class::cautionary)
		scoreManager->IncrementScore(2);
	if (someAstroid->weightClass == size_class::insignifigant)
		scoreManager->IncrementScore(1);


	localGameObjects.erase(std::remove(localGameObjects.begin(), localGameObjects.end(), someAstroid), localGameObjects.end());

}

std::string MissionSurvival::getTimeLeft_string()
{
	return std::string();
}

float MissionSurvival::getTimeLeft_float()
{
	return 0.0f;
}
