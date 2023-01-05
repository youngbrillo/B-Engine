#include "MissionStandard.h"

#include "AstroidObject.h"
#include <imgui.h>
#include "App.h"
#include "Game.h"


MissionStandard::MissionStandard(MissionDefinition* def)
	: Mission(def)
	, targetsEliminated(0)
	, targetsGoal(def->ClearRequirements)
	, statusString("Active: ")
	, spawnOffset(b2Vec2_zero)
	, exitBody(nullptr)
	, contrainObjectsToScreen(def->contrainObjectsToScreen)
{
	//printf("PLEASE ...., setup a listener in the objectFactory that this class can tune into to get info about what objects have been deleted.... thank you \n");
	if (App::g_App->state != CSApp::App_State::state_running)
	{
		App::g_App->state = CSApp::App_State::state_running;
		
	}
	b2BodyDef bd;
		bd.position = def->endPos;
		bd.type = b2BodyType::b2_staticBody;
		bd.fixedRotation = true;
	exitBody = Game::m_World->CreateBody(&bd);
	b2FixtureDef fd;
		b2CircleShape shape;
		shape.m_radius = 3.0f;

		fd.isSensor = true;
		fd.shape = &shape;
	exitBody->CreateFixture(&fd);
}

MissionStandard::~MissionStandard()
{
	localGameObjects.clear();
	Game::m_World->DestroyBody(exitBody);

	exitBody = nullptr;
}

void MissionStandard::Update(float deltaTime)
{
	repositionOffscreenObjects();
	if (hasEnded) return;
	Mission::Update(deltaTime);

	if(returnedEarly) {
		this->onEnd();
	}



}

void MissionStandard::onBeginContact(b2Contact* contact)
{
	b2Fixture* A = contact->GetFixtureA();
	b2Fixture* B = contact->GetFixtureB();
	b2Fixture* other = nullptr;
	if (A->GetBody() == this->exitBody)
	{
		other = B;
	}
	else if (B->GetBody() == this->exitBody)
	{
		other = A;
	}
	if (other == nullptr) return;

	collisionType_astroids* ct = (collisionType_astroids*)other->GetUserData();
	bool canContinue = !!ct;
	if (targetsEliminated >= targetsGoal && canContinue)
	{
		if(*ct == collisionType_astroids::collision_ship)
			returnedEarly = true;
	}


}

void MissionStandard::onEndContact(b2Contact* contact)
{
}


void MissionStandard::onEnd()
{
	hasEnded = true;
	if (state == ms_Success) 
	{
		//
		statusString = "succeded this mission! some success huh!";
	}
	else if (state == ms_Failed || state == ms_Active)
	{
		//timed out here, possible that did not fail if eliminated > left
		if (targetsEliminated >= targetsGoal)
		{
			state = ms_Success;
			statusString = "succeded this mission! you met the goal";
		}
		else
		{
			statusString = "failed this mission! you gotta be faster than that...";
		
		}
	}
	
	//App::g_App->state = CSApp::App_State::state_paused;
}

void MissionStandard::Debug()
{
	if (ImGui::TreeNode("- Mission: Standard -"))
	{
		ImGui::SliderInt("Targets eliminated", &targetsEliminated, 0, targetsGoal);
		ImGui::SliderInt("Target Goal", &targetsGoal, targetsGoal, targetsGoal);
		ImGui::SliderFloat2("spawn offset", &spawnOffset.x, 0, 10.0f);
		
		
		Mission::Debug();
		ImGui::TreePop();
	}
}

void MissionStandard::DrawUI()
{
	DrawString(500, 0, getTimeLefts().c_str());
	DrawString(500, 20, statusString.c_str());

}

void MissionStandard::DrawContent()
{

}

void MissionStandard::ObjectCreated(GameObject* G)
{
	AstroidObject* someAstroid = dynamic_cast<AstroidObject*> (G);
	if (someAstroid)
	{
		localGameObjects.emplace_back(someAstroid);
	}
}

#include <sstream>
void MissionStandard::ObjectDeleted(GameObject* G)
{
	AstroidObject* someAstroid = dynamic_cast<AstroidObject*> (G);

	if (someAstroid)
	{
		targetsEliminated++;
		std::stringstream ss;
		std::string _state;
		_state = this->state == MissionState::ms_Active ? "Active: " : this->state == MissionState::ms_Success ? "Completed: " : "Failed: ";
		ss << _state << targetsEliminated << "/" << targetsGoal;
		statusString = ss.str();

		localGameObjects.erase(std::remove(localGameObjects.begin(), localGameObjects.end(), someAstroid), localGameObjects.end());

	}
}

void MissionStandard::repositionOffscreenObjects()
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

std::string MissionStandard::getTimeLefts()
{
	float timeLeft = timeLimit - timeElapsed;
	std::stringstream ss;

	//get minutes left
	ss << (int) timeLeft /60 << ":";
	//get seconds
	ss << (int)timeLeft % 60;
	return ss.str();
}

float MissionStandard::getTimeLeftf()
{
	return timeLimit - timeElapsed;
}
