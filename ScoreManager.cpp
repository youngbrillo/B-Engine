#include "ScoreManager.h"
#include <stdio.h>
float base_ComboMultiplier = 2.0f;

SpaceBuster::ScoreManager::ScoreManager(float comboLength, int levelGoal)
	: score(0)
	, scoreMultiplier(1)
	, comboProgress(0.0f)
	, comboMaxLength(comboLength)
	, comboMultiplier(base_ComboMultiplier)
	, comboValid(false)
	, comboLevel(comboTier::ctLevelOne)
	, nextLevelProgress(0)
	, nextLevelGoal(levelGoal)
	, debugIndex(0)
{
}

SpaceBuster::ScoreManager::~ScoreManager()
{
}

void SpaceBuster::ScoreManager::onScore(int baseIncrement)
{
	float incrementation = baseIncrement * scoreMultiplier;
	if (comboValid)
	{
		incrementation *= comboMultiplier * (int)comboLevel;
	}

	score += (int) incrementation;
}

void SpaceBuster::ScoreManager::IncrementScore()
{
	onScore(1);
	comboProgress = 0.0f;
	comboValid = true;
	nextLevelProgress++;
	if (nextLevelProgress == nextLevelGoal)
	{
		advanceLevels(comboLevel);
		nextLevelProgress = 0;
	}

}

void SpaceBuster::ScoreManager::onWaveClear()
{
	scoreMultiplier++;
}

void SpaceBuster::ScoreManager::Update(float dt)
{
	if (comboValid)
	{
		comboProgress += dt;
		if (comboProgress >= comboMaxLength)
		{
			comboProgress = 0.0f;
			comboMultiplier = base_ComboMultiplier;
			comboLevel = comboTier::ctLevelOne;
			comboValid = false;
		}
	}
}
#include <imgui.h>

const SpaceBuster::comboTier _ComboTiers[5] =
{ SpaceBuster::comboTier::ctLevelOne, SpaceBuster::comboTier::ctLevelTwo, SpaceBuster::comboTier::ctLevelThree, SpaceBuster::comboTier::ctLevelFour, SpaceBuster::comboTier::ctLevelFive };
const char* _ComboTierNames[5] =
{ "Level one",  "Level two" ,  "Level three" ,  "Level four" ,  "Level five" };

void SpaceBuster::ScoreManager::Debug(const char* name)
{
	if (ImGui::TreeNode(name))
	{
		ImGui::Checkbox("Combo Active", &comboValid);

		ImGui::SliderInt("Score", &score, score, score);
		ImGui::SliderInt("scoreMultiplier", &scoreMultiplier, scoreMultiplier, scoreMultiplier);
		ImGui::SliderInt("nextLevelProgress", &nextLevelProgress, 0, nextLevelGoal);
		ImGui::SliderInt("nextLevelGoal", &nextLevelGoal, nextLevelGoal, nextLevelGoal);

		ImGui::SliderFloat("comboProgress", &comboProgress, 0.0f, comboMaxLength);
		ImGui::SliderFloat("comboMaxLength", &comboMaxLength, comboMaxLength, comboMaxLength);
		ImGui::SliderFloat("comboMultiplier", &comboMultiplier, 0, 10);


		ImGui::TreePop();
	}
}
#include <sstream>
#include "Utility_Physics.h"
void SpaceBuster::ScoreManager::Draw(int x, int y)
{
	std::stringstream ss;
	ss << "Score: " << this->score;
	DrawString(x, y, ss.str().c_str());

}


//#include "AstroidObject.h"
//void SpaceBuster::ScoreManager::ObjectCreated(GameObject* G)
//{
//}
//
//void SpaceBuster::ScoreManager::ObjectDeleted(GameObject* G)
//{
//	AstroidObject* someAstroid = dynamic_cast<AstroidObject*> (G);
//	if (someAstroid)
//	{
//		onAstroidDestroy();
//	}
//
//}

void SpaceBuster::ScoreManager::advanceLevels(comboTier& tier)
{
	switch (tier)
	{
		case ctLevelOne: 
			tier = ctLevelTwo;
			comboMultiplier = 2.5;
			break;
		case ctLevelTwo: 
			tier = ctLevelThree;
			comboMultiplier = 3.4;
			break;
		case ctLevelThree: 
			tier = ctLevelFour;
			comboMultiplier = 4.3f;
			break;
		case ctLevelFour: 
			tier = ctLevelFive;
			comboMultiplier = 6.0f;
			break;
		case ctLevelFive: 
			comboMultiplier = 10.0f;
			printf("At max combo level!\n");
			break;
	default:
		break;
	}
}
