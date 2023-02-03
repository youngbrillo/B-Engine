#include "Game.h"

#include "imgui.h"
#include "b2BodyGrabber.h"
#include "Ship.h"
#include "OmniLauncher.h"
#include "NeoShipController.h"
#include "AstroidManager.h"
#include "ObjectFactory.h"
#include "UI_Button.h"
#include "ResourceManager.h"

class ShipConfigScene2 : public Game
{
public:
	b2BodyGrabber* grabber;
	glm::vec2 mousePos;
	NeoShipController* ship;
	SpaceBuster::AstroidFactory* afactory;
	//constructor
	ShipConfigScene2() : Game()
		, mousePos(0, 0)
	{
		AppCam->zoom = 2.358;

		m_World->SetGravity(b2Vec2(0, 0));
		//create ground
		b2BodyDef bd;
		{
			bd.position = b2Vec2(0.0f, -20.0f);
			bd.type = b2BodyType::b2_staticBody;
			m_groundBody = m_World->CreateBody(&bd);
			float extents = 90;
			{
				b2EdgeShape shape;
				//shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
				shape.Set(b2Vec2(-extents, 0.0f), b2Vec2(extents, 0.0f));
				m_groundBody->CreateFixture(&shape, 0.0f);

				//shape.Set(b2Vec2( 40.0f, 40.0f), b2Vec2(40.0f, 0.0f));
				shape.Set(b2Vec2(extents, extents), b2Vec2(extents, 0.0f));
				m_groundBody->CreateFixture(&shape, 0.0f);

				//shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(-40.0f, 40.0f));
				shape.Set(b2Vec2(-extents, 0.0f), b2Vec2(-extents, extents));
				m_groundBody->CreateFixture(&shape, 0.0f);

				//shape.Set(b2Vec2(-40.0f, 40.0f), b2Vec2(40.0f, 40.0f));
				shape.Set(b2Vec2(-extents, extents), b2Vec2(extents, extents));
				m_groundBody->CreateFixture(&shape, 0.0f);
			}


		}



		grabber = new b2BodyGrabber(*m_World);
		NeoShipControllerDefinition def;
		ship = new NeoShipController(&def, m_groundBody);


		afactory = new SpaceBuster::AstroidFactory(5U);
		m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);

		//UIElement::defaultTexture = ResourceManager::GetTexturePtr("default");
	};
	//destructor
	~ShipConfigScene2()
	{
		delete grabber;
		delete ship;
		delete afactory;
	};

	//key callbacks and frame input key callback
	void KeyboardUp(int key) override 
	{ 
		ship->onKeyRelease(key);
	}
	void KeyboardDown(int key)override 
	{ 
		ship->onKeyPress(key);


	}
	virtual void processInput(GLFWwindow* window, float deltaTime) override { }
	//mouse callbacks
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override 
	{
		ship->mouseCallback(button, action, mode);
		if (button == GLFW_MOUSE_BUTTON_1)
		{

			b2Vec2 _wp(mousePos.x, mousePos.y);
			if (action == GLFW_PRESS)
			{
				grabber->grabRigidBody(_wp);
			}

			if (action == GLFW_RELEASE)
			{
				grabber->releaseRigidBody(_wp);
			}
			if (action == GLFW_RELEASE && mode == GLFW_MOD_SHIFT)
			{
				grabber->toggleRigidBodyType(_wp);
			}
		}
		
	}
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) override 
	{
		ship->mouseCallback_Cursor(x, y);
		{
			b2Vec2 _wp(mousePos.x, mousePos.y);
			grabber->mouseWorldSpacePosition = _wp;
			grabber->moveRigidBody(_wp);
		}
	}
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override 
	{
	
	}
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override {}

	//frame updates
	void FixedUpdate(float dt) override 
	{ 
		ship->FixedUpdate(dt);
		ObjectFactory::FixedUpdateObjects(dt);
	}
	void Update(float dt) override 
	{
		ship->Update(dt);
		//double x, y;
		//glfwGetCursorPos(Game::mainWindow, &x, &y);

		//glm::vec2 mpw = glm::vec2((float)x, (float)y);
		//mousePos = Game::AppCam->convertScreenToWorld(mpw);
		//ship->target.x = mousePos.x; ship->target.y = mousePos.y;
		mousePos = Game::AppCam->convertScreenToWorld(ship->mouse_position);
		afactory->Update(dt);
		ObjectFactory::UpdateObjects(dt);
	}

	//collision callbacks 
	void onBeginContact(b2Contact* contact) override { ship->handleBeginContact(contact); ObjectFactory::handleBeginContact(contact); }
	void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override 
	{ 
		ship->handlePreSolve(contact, oldManifold); 
		ObjectFactory::handlePreSolve(contact, oldManifold);
	}
	void onEndContact(b2Contact* contact) override { 
		ship->handleEndContact(contact); 
		ObjectFactory::handleEndContact(contact);
	}
	void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override { 
		ship->handlePostSolve(contact, impulse); 
		ObjectFactory::handlePostSolve(contact, impulse);
	}

	//screen drawing
	void DrawBackGround() override {}
	void DrawForeGround() override 
	{
		ObjectFactory::DrawObjects(m_Shader, m_surface);
		ship->Draw(m_Shader, m_surface);
	}
	//void DrawScene() override {} 
	void DrawDebug() override 
	{
		ship->Debug();
		m_physDrawer->renderDebug();
		grabber->debug();
		ObjectFactory::Debug();
	}

	//creation
	static Game* Create() { return new ShipConfigScene2; }
};

static int testIndex = RegisterGame("SpaceBuster", "Ship Config (2)", ShipConfigScene2::Create); 
