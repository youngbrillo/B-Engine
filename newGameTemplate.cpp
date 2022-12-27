#include "Game.h"

#include "imgui.h"

class NewGameTemplate : public Game
{
public:
	//constructor
	NewGameTemplate() : Game()
	{

	};
	//destructor
	~NewGameTemplate()
	{

	};

	//key callbacks and frame input key callback
	void KeyboardUp(int key) override { }
	void KeyboardDown(int key)override { }
	virtual void processInput(GLFWwindow* window, float deltaTime) override { }
	//mouse callbacks
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override {}
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) override {}
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override {}
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override {}

	//frame updates
	void FixedUpdate(float dt) override { }
	void Update(float dt) override { }

	//collision callbacks 
	void onBeginContact(b2Contact* contact) override {}
	void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override {}
	void onEndContact(b2Contact* contact) override {}
	void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override {}

	//screen drawing
	void DrawBackGround() override {}
	void DrawForeGround() override {}
	//void DrawScene() override {} 
	void DrawDebug() override {}

	//creation
	static Game* Create() { return new NewGameTemplate; }
};

//static int testIndex = RegisterGame("Game", "SpaceBuster III", NewGameTemplate::Create);  //do not actually register this lol
