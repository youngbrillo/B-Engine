#include "Game.h"
#include "SpaceBusterGame.h"
#include "ObjectFactory.h"

#include "imgui.h"

class ShipConfigScene : public SpaceBusterGame
{
public:
	//constructor
	ShipConfigScene() : SpaceBusterGame()
	{
		m_astroidFactory->autoGenerateEnabled = false;
		m_cameraController->setEnabled(false);
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
	}
	//void DrawScene() override {} 
	void DrawDebug() override {
		SpaceBusterGame::DrawDebug();
	}

	//creation
	static Game* Create() { return new ShipConfigScene; }
};

static int testIndex = RegisterGame("SpaceBuster", "Ship Config", ShipConfigScene::Create);  
