#include "Game.h"
#include "SpaceBusterGame.h"

#include "imgui.h"

//add a survival mission to this scene
//implement power ups for ship/all ships
//add UI
//implement high score


class SpaceBuster_GameLoop_Test : public SpaceBusterGame
{
public:
	//constructor
	SpaceBuster_GameLoop_Test() : SpaceBusterGame()
	{
		m_cameraController->mode = CameraController::FollowMode::Lerp;
	};
	//destructor
	~SpaceBuster_GameLoop_Test()
	{

	};

	//key callbacks and frame input key callback
	//void KeyboardUp(int key) override { }
	//void KeyboardDown(int key)override { }
	//virtual void processInput(GLFWwindow* window, float deltaTime) override { }
	//mouse callbacks
	//virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override {}
	//virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) override {}
	//virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override {}
	//virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override {}

	//frame updates
	//void FixedUpdate(float dt) override { }
	void Update(float dt) override 
	{ 
		SpaceBusterGame::Update(dt);
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

	}


	void DrawDebug() override {
		SpaceBusterGame::DrawDebug();
	}

	//creation
	static Game* Create() { return new SpaceBuster_GameLoop_Test; }
};

static int testIndex = RegisterGame("SpaceBuster", "Game loop", SpaceBuster_GameLoop_Test::Create);  //do not actually register this lol
