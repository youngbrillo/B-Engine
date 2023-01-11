#include "SpaceBusterGame.h"

#include "imgui.h"
#include "Utility_Physics.h"
#include "ObjectFactory.h"
#include "ResourceManager.h"

#include "Mission.h"
#include "MissionStandard.h"
#include "App.h"

class MissionScene : public SpaceBusterGame
{
public:
	GameObject* Destination;
	Mission* currentMission;
	MissionDefinition mission1;
	//constructor
	MissionScene() : SpaceBusterGame()
	{


		//b2Body* ground = generateBounds_rect(m_World, b2Vec2(0.0f, 20.0f), 50, -50, 1.0f, 0.0f, 0.0f);
		m_astroidFactory->max = 10;
		m_astroidFactory->SpawnTimeTarget = 0.5f;
		m_astroidFactory->maxAstroidSize = 5.0f;

		//setup mission
		{
			mission1.title = "Mission 00: Destroy 100 Astroids";
			mission1.desc = "Destroy 100 Astroids before time runs out!!!";
			mission1.endPos = b2Vec2(0.0f, 20.0f);
			mission1.defaultTimeLimit = 30.0f;
			mission1.hasTimeLimit = true;
			mission1.contrainObjectsToScreen = true;
			mission1.ClearRequirements = 5;
		}
		currentMission = new MissionStandard(&mission1);

		ObjectFactory::SetObjectListener(currentMission);


		b2Fixture* fix = m_groundBody->GetFixtureList();
		while (fix != nullptr)
		{
			b2Fixture* td = fix;
			fix = fix->GetNext();

			m_groundBody->DestroyFixture(td);
		}
		//m_astroidFactory->createAstroid(b2Vec2(3, 0.0f), b2Vec2(0.0f, -10.0f), 0, b2Vec2(2.5f, 2.5f));
	};
	//destructor
	~MissionScene()
	{
		//delete Destination; //deleted by objectFactory
		ObjectFactory::SetObjectListener(nullptr);
		delete currentMission;
	};

	//key callbacks and frame input key callback
	//void KeyboardUp(int key) override { }
	void KeyboardDown(int key)override 
	{ 
		SpaceBusterGame::KeyboardDown(key);

		if (key == GLFW_KEY_L )
		{
			if (currentMission)
			{
				if (currentMission->queryMissionStatus() != MissionState::ms_Active)
				{
					delete currentMission;

					currentMission = new MissionStandard(&mission1);
					ObjectFactory::SetObjectListener(currentMission);
				}
			}
			else
			{
				currentMission = new MissionStandard(&mission1);
				ObjectFactory::SetObjectListener(currentMission);
			}
		}
		if (currentMission)
		{
			if (key == GLFW_KEY_O && currentMission->queryMissionEnd())
			{
				delete currentMission;
				currentMission = nullptr;
				ObjectFactory::SetObjectListener(nullptr);
			}
		}
	}
	//virtual void processInput(GLFWwindow* window, float deltaTime) override { }
	////mouse callbacks
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override 
	{
		SpaceBusterGame::mouseCallback(window, button, action, mode);

		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
		{
			///printf("Why'd you click the mouse bro...\n");
		}
	}
	//virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) override {}
	//virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override {}
	//virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override {}

	//frame updates
	void FixedUpdate(float dt) override 
	{ 
		SpaceBusterGame::FixedUpdate(dt);
		if(currentMission) currentMission->FixedUpdate(dt);

	}
	void Update(float dt) override 
	{ 
		SpaceBusterGame::Update(dt);
		if (currentMission) {
			currentMission->Update(dt);
			if (currentMission->queryMissionEnd())
			{
				//if (currentMission->queryMissionStatus() == MissionState::ms_Success)
				//{
					//currentMission = nullptr;
				//}
			}
		}

	}

	//collision callbacks 
	void onBeginContact(b2Contact* contact) override 
	{
		SpaceBusterGame::onBeginContact(contact);
		if (currentMission) currentMission->onBeginContact(contact);
	}
	void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override 
	{
		SpaceBusterGame::onPreSolve(contact, oldManifold);
		if (currentMission) currentMission->onPreSolve(contact, oldManifold);

	}
	void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override 
	{
		SpaceBusterGame::onPostSolve(contact, impulse);
		if (currentMission) currentMission->onPostSolve(contact, impulse);
	}
	void onEndContact(b2Contact* contact) override
	{
		SpaceBusterGame::onEndContact(contact);
		if (currentMission) currentMission->onEndContact(contact);
	}

	//screen drawing
	void DrawBackGround() override {}
	void DrawForeGround() override {
		SpaceBusterGame::DrawForeGround();

		DrawString(05, 0, "Controls:");
		DrawString(30, 20, "W - Forward");
		DrawString(30, 40, "S - Backward");
		DrawString(30, 60, "A/D - rotate Left/Right");


		if (currentMission) currentMission->DrawUI();
	}
	//void DrawScene() override {} 
	void DrawDebug() override 
	{
		if (currentMission) currentMission->Debug();
		SpaceBusterGame::DrawDebug();

	}

	//creation
	static Game* Create() { return new MissionScene; }
};

static int testIndex = RegisterGame("SpaceBuster", "Mission Scene", MissionScene::Create);  //do not actually register this lol
