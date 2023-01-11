#include "Game.h"
#include "SpaceBusterGame.h"
#include "ObjectFactory.h"

#include "imgui.h"
#include "UI_Meter.h"

class ShipConfigScene : public SpaceBusterGame
{
public:
	//constructor

	//create ship ui
	UIMeter m_energyMeter, m_healthMeter, m_ammoMeter;
	float m_min;
	int	 m_int_min;
	Shader* uiShader;
	ShipConfigScene() : SpaceBusterGame()
	{
		m_astroidFactory->autoGenerateEnabled = false;
		m_cameraController->setEnabled(false);

		//set up ui element
		m_min = 0.0f;
		m_int_min = 0;
		UIElementDefinition UIdef;
		{ //energy meter
			UIdef.position = glm::vec3(-40.0f, 16.0f, 0.0f);
			//UIdef.color = glm::vec4(0.353f, 1.0f, 0.0f, 1.0f);
			UIdef.elementName = "Energy bar test";
			m_energyMeter = UIMeter(&UIdef, &m_player->m_ship->energy, &m_min, &m_player->m_ship->maxEnergy);
		}
		glm::vec3 offset = glm::vec3(0, 2.0f, 0.0f);

		{ //health meter
			UIdef.position = UIdef.position + offset;
			//UIdef.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			UIdef.elementName = "condition bar test";
			m_healthMeter = UIMeter(&UIdef, &m_player->m_ship->condition, &m_min, &m_player->m_ship->maxCondition);
		}

		{ //ammo meter
			UIdef.position = UIdef.position + offset;
			//UIdef.color = glm::vec4(0.0f, .0f, 1.0f, 1.0f);
			UIdef.elementName = "ammo bar test";
			m_ammoMeter = UIMeter(&UIdef, &m_player->m_launcher->magCurrent, &m_int_min, &m_player->m_launcher->magCapacity);
		}
		//setup ui shaders
		uiShader = new Shader("UIShader00.vts", "test.frs", true);
		glm::mat4  proj = AppCam->getProjectionMatrix() , view = glm::mat4(1.0f);
		uiShader->Use().SetMatrix4("projection", proj).SetMatrix4("view", view);
	};




	//destructor
	~ShipConfigScene()
	{

	};

	//key callbacks and frame input key callback
	void KeyboardUp(int key) override { 
		SpaceBusterGame::KeyboardUp(key); 
	}
	void KeyboardDown(int key)override { 
		SpaceBusterGame::KeyboardDown(key);
	}
	virtual void processInput(GLFWwindow* window, float deltaTime) override { }
	//mouse callbacks
	//frame updates
	//collision callbacks 

	//screen drawing
	void DrawBackGround() override {}
	void DrawForeGround() override {
		SpaceBusterGame::DrawForeGround();
		m_energyMeter.Draw(uiShader, m_surface);
		m_healthMeter.Draw(uiShader, m_surface);
		m_ammoMeter.Draw(uiShader, m_surface);

	}
	//void DrawScene() override {} 
	void DrawDebug() override {
		m_energyMeter.Debug();
		m_healthMeter.Debug();
		m_ammoMeter.Debug();
		SpaceBusterGame::DrawDebug();
	}

	//creation
	static Game* Create() { return new ShipConfigScene; }
};

static int testIndex = RegisterGame("SpaceBuster", "Ship Config", ShipConfigScene::Create);  
