#include "Game.h"


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <math.h>

#include "Utility_Physics.h"
#include "ShipController.h"
#include "AstroidManager.h"
#include "CameraController.h"
#include "Zone.h"
#include <sstream>
#include "ObjectFactory.h"

using namespace SpaceBuster;

class SpaceBusterIII : public Game
{
public:
	//members
	b2Body* b_bounds; b2Vec2 vbounds, pBounds;
	ShipController* ship;
	Zone* leftZone;
	AstroidFactory* astroidManager;
	CameraController cameraController;
	//constructor
	SpaceBusterIII() : Game(), vbounds(90.0f, 40.0f), pBounds(0.0f, 20.0f)
	{
		m_World->SetGravity(b2Vec2(0.0f, 0.0f));
		AppCam->position = glm::vec3(0.0f, 20.0f, 0.0f);
		AppCam->zoom = 2.359f;//1.772f;
		Game::windowColorBackGround = glm::vec4(0, 0, 0, 1.0f);



		b_bounds = generateBounds_rect(m_World, pBounds, vbounds.x, vbounds.y,0.40f); //create ground to simulate friction against
		ship = new ShipController(m_World, b2Vec2_zero, b_bounds);
		astroidManager = new AstroidFactory(25); astroidManager->autoGenerateEnabled = false;
		ShipController::g_shipController = ship;

		cameraController = CameraController(Game::AppCam, ship->getBody());
		cameraController.mode = CameraController::FollowMode::Bound;
		//create astroids
		{
			b2Vec2 pos, linVel = b2Vec2_zero;float angVel;
		
			pos = b2Vec2(0.0f, 20.0f);
			//linVel = b2Vec2(-15.f, -10.0f);
			angVel = 5.0f;
			astroidManager->createAstroid(pos, linVel, angVel, b2Vec2(25/2,25/2));

		}
		//create zone // a sort of, goal post if you  will
		{
			b2FixtureDef fd;
			b2PolygonShape shape;
			shape.SetAsBox(20/2.0f, 50/2.0f);

			fd.shape = &shape;
			leftZone = new Zone(false, b2Vec2(-80.0f, 20.0f), 5, 20, 50, &fd);

		}

		m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);

		GameObject::g_GameObjectSurface = this->m_surface;
		GameObject::g_gameObjectShader = this->m_Shader;
	};
	 ~SpaceBusterIII()
	{
		delete ship;
		m_World->DestroyBody(b_bounds);
		b_bounds = NULL;

		delete astroidManager;

		GameObject::g_GameObjectSurface = nullptr;
		GameObject::g_gameObjectShader = nullptr;
	};


	void KeyboardUp(int key) override
	{
		ship->onKeyRelease(key);
	}
	void KeyboardDown(int key) {
		ship->onKeyPress(key);
	}

	void FixedUpdate(float dt) override
	{
		Game::FixedUpdate(dt);
		ship->FixedUpdate(dt);
		leftZone->FixedUpdate(dt);
	}

	void Update(float dt) override
	{
		Game::Update(dt);
		ship->Update(dt);
		astroidManager->Update(dt);
		leftZone->Update(dt);
		cameraController.Update(dt);
	}

	// Implement contact listener.
	void onBeginContact(b2Contact* contact) override
	{
		Game::onBeginContact(contact);
		leftZone->handleBeginContact(contact);
		ship->handleBeginContact(contact);
	}

	void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
	{
		Game::onPreSolve(contact, oldManifold);
		ship->handlePreSolve(contact, oldManifold);
		cameraController.handlePreSolve(contact, oldManifold);
		leftZone->onPreSolve(contact, oldManifold);

	}

	// Implement contact listener.
	void onEndContact(b2Contact* contact) override
	{
		Game::onEndContact(contact);
		leftZone->handleEndContact(contact);
		ship->handleEndContact(contact);
	}

	void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
	{
		Game::onPostSolve(contact, impulse);
		ship->handlePostSolve(contact, impulse);
	}

	void DrawDebug() override {

		if (ImGui::SliderFloat2("bounds pos", &pBounds.x, -50, 50))
		{
			m_World->DestroyBody(b_bounds);
			b_bounds = generateBounds_rect(m_World, pBounds, vbounds.x, vbounds.y);
			attachFrictionJoint(b_bounds, ship->getBody());
		}

		if (ImGui::SliderFloat2("bounds config", &vbounds.x, 0, 350))
		{
			m_World->DestroyBody(b_bounds);
			b_bounds = generateBounds_rect(m_World, pBounds, vbounds.x, vbounds.y);
			attachFrictionJoint(b_bounds, ship->getBody());
		}

		leftZone->Debug();
		cameraController.Debug();

		ship->Debug();
		astroidManager->Debug();
	}


	void DrawBackGround() override
	{
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		DrawString(5, 0, "WASD to control the ship");
		DrawString(5, 15, "[space]: shoot");
		DrawString(5, 30, "objective: Collect materials from the destroyed astroids.");
		DrawString(5, 45, "to rebuild your mothership and escape the astroid belt");
	}

	void DrawForeGround() override
	{
		ObjectFactory::DrawObjects();
		std::stringstream info;
		info << "Amount of Astroids Destroyed: ";
		info << astroidManager->amountDeleted;

		DrawString(Game::AppCam->Width/2, 0, info.str().c_str(), " that is all");
	}

	static Game* Create() { return new SpaceBusterIII; }
};

static int testIndex = RegisterGame("Game", "SpaceBuster III", SpaceBusterIII::Create);
