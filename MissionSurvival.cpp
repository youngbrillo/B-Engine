#include "MissionSurvival.h"
#include "AstroidObject.h"
#include <imgui.h>

MissionSurvival::MissionSurvival(MissionDefinition* def)
	: Mission(def)
	, uiPos({10, 10})
{
	scoreManager = new SpaceBuster::ScoreManager(2.0f, 10);
}

MissionSurvival::~MissionSurvival()
{
	delete scoreManager;

}

void MissionSurvival::Update(float deltaTime)
{
	scoreManager->Update(deltaTime);
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
	scoreManager->Debug();
	if(ImGui::TreeNode("- Mission: Survive -"))
	{
		ImGui::SliderInt2("UI Position", &uiPos.x, 0, 100);
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
}

void MissionSurvival::ObjectDeleted(GameObject* G)
{
	AstroidObject* someAstroid = dynamic_cast<AstroidObject*> (G);
	if (!someAstroid) return;

	scoreManager->IncrementScore();
}
