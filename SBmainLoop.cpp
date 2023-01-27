#include "Game.h"
#include "SpaceBusterGame.h"

#include "imgui.h"
#include "ScoreManager.h"
#include "ObjectFactory.h"
#include "MissionSurvival.h"
//implement high score 1st
//add UI 0th
//add a survival mission to this scene 2nd
//implement power ups for ship/all ships 3rd


class SpaceBuster_GameLoop_Test : public SpaceBusterGame
{
public:
	//SpaceBuster::ScoreManager* m_scoreKeeper;
	Mission* mission;
	//constructor
	SpaceBuster_GameLoop_Test() : SpaceBusterGame()
	{
		m_cameraController->mode = CameraController::FollowMode::Bound;
		Game::windowColorBackGround = glm::vec4(0.451, 0.252, 0.252, 1.00f);

		MissionDefinition def;

		mission = new MissionSurvival(&def);
		ObjectFactory::SetObjectListener(mission);
	};
	//destructor
	~SpaceBuster_GameLoop_Test()
	{
		ObjectFactory::SetObjectListener(nullptr);
		delete mission;
	};

	//frame updates
	void Update(float dt) override 
	{ 
		SpaceBusterGame::Update(dt);
		mission->Update(dt);
	}

	//collision callbacks 
	//void onBeginContact(b2Contact* contact) override {}
	//void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override {}
	//void onEndContact(b2Contact* contact) override {}
	//void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override {}

	//screen drawing
	void DrawBackGround() override 
	{
	
	}
	void DrawForeGround() override 
	{
		SpaceBusterGame::DrawForeGround();
		mission->DrawContent();
		mission->DrawUI();
	}


	void DrawDebug() override {
		mission->Debug();
		SpaceBusterGame::DrawDebug();
	}

	//creation
	static Game* Create() { return new SpaceBuster_GameLoop_Test; }
};

static int testIndex = RegisterGame("SpaceBuster", "Game loop", SpaceBuster_GameLoop_Test::Create);  //do not actually register this lol
