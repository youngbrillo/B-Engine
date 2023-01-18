#include "Game.h"
#include "ShipController.h"
#include "AstroidManager.h"
#include <sstream>

#include "imgui.h"
#include "Item.h"
#include "ItemObject.h"
#include "ResourceManager.h"
#include "ObjectFactory.h"

using namespace SpaceBuster;

class ItemPickupScene : public Game
{
public:
	//member variables
	ShipController* m_player;
	AstroidFactory* m_astroidFactory;
	b2Body* b_bounds;
	glm::vec3 fricProp;//(10.0f, 0.5f, 0.2f);
	b2Joint* m_fricJoint;

	b2Vec2 nextItemPos = b2Vec2_zero, nextItemSpeed = b2Vec2_zero;
	ItemType currentItemType;

	//member functions

	//add gameobject vector to base when kinks get ironed out


	ItemPickupScene() : Game()
	{
		m_World->SetGravity(b2Vec2(0.0f, 0.0f));
		AppCam->position = glm::vec3(0.0f, 20.0f, 0.0f);
		AppCam->zoom = 2.359f;//1.772f;
		//Game::windowColorBackGround = glm::vec4(0, 0, 0, 1.0f);
		fricProp = glm::vec3(10.0f, 0.5f, 0.2f);
		//fricProp = glm::vec3(21.364f, 1.5f, 0.25f);

		currentItemType = ItemType::SparePart;

		//dont start up the resource manager here, it is inefficient!, instead load it at the app start time
		//ResourceManager::startup();

		{
			b2BodyDef bd;
			bd.position = b2Vec2(0.0f, 20.0f);
			bd.type = b2BodyType::b2_staticBody;

			b_bounds = m_World->CreateBody(&bd);
			b_bounds = generateBounds_rect(m_World, b2Vec2(0.0f, 20.0f), 90.0f, 40.0f, 0.40f); //create ground to simulate friction against

		}

		m_player = new ShipController(m_World, b2Vec2_zero);
		ShipController::g_shipController = m_player;

		//can't move?
		//gameFeelisJackedUpMan!!
		m_fricJoint = attachFrictionJoint2(b_bounds, m_player->getBody(), fricProp.x, fricProp.y, fricProp.z);



		m_astroidFactory = new AstroidFactory(1); m_astroidFactory->autoGenerateEnabled = true;
		m_astroidFactory->createAstroid(b2Vec2(-20, 30), b2Vec2(-20, 30), 0, b2Vec2(2.0f, 2.f));

		m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);

		GameObject::g_GameObjectSurface = this->m_surface;
		GameObject::g_gameObjectShader = this->m_Shader;
	}
	~ItemPickupScene() {
		ShipController::g_shipController = nullptr;

		delete m_player;
		delete m_astroidFactory;
		b_bounds = nullptr;


		GameObject::g_GameObjectSurface = nullptr;
		GameObject::g_gameObjectShader = nullptr;

		//ResourceManager::cleanup();

	}

	// solvers ---- ---- ---- ---- ---- ---- ---- ----- ---- ---- ---- ---- --- ---- ---- --- ---- ----

	void KeyboardUp(int key) override
	{
		m_player->onKeyRelease(key);
	}
	void KeyboardDown(int key) {
		m_player->onKeyPress(key);
	}

	void FixedUpdate(float dt) override
	{
		Game::FixedUpdate(dt);
		m_player->FixedUpdate(dt);
	}

	void Update(float dt) override
	{
		Game::Update(dt);
		m_player->Update(dt);
		m_astroidFactory->Update(dt); //to do, remove this guy too
	}

	// Implement contact listener.
	void onBeginContact(b2Contact* contact) override
	{
		Game::onBeginContact(contact);
		m_player->handleBeginContact(contact);
	}

	void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
	{
		Game::onPreSolve(contact, oldManifold);
		m_player->handlePreSolve(contact, oldManifold);

	}

	// Implement contact listener.
	void onEndContact(b2Contact* contact) override
	{
		Game::onEndContact(contact);
		m_player->handleEndContact(contact);
	}

	//to do remove once the astroid factor starts to insert into the object factory
	void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
	{
		Game::onPostSolve(contact, impulse);
		m_player->handlePostSolve(contact, impulse);
	}


	void DrawDebug() override {

		ImGui::SliderFloat2("Next item pos", &nextItemPos.x, -30, 30);
		ImGui::SliderFloat2("Next item vel", &nextItemSpeed.x, -30, 30);
		
		static int elem = (int)ItemType::null_item; //b/c its first
		const char* elem_names[(int)ItemType::tool_gravityBomb + 1] = {"N/A", "Spare Parts", "Energy Cell", "Soft Ammo", "Reflective Ammo"};
		const char* elem_name = (elem >= 0 && elem < (int)ItemType::tool_gravityBomb + 1) ? elem_names[elem] : "Unknown";

		elem = (int)this->currentItemType;
		if (ImGui::SliderInt("State", &elem, 0, (int)ItemType::tool_gravityBomb, elem_name)) {
			this->currentItemType = (ItemType)elem;
		}

		if (ImGui::Button("Generate"))
		{
			GameObjectDefinition gd;
			gd.position = glm::vec3(nextItemPos.x, nextItemPos.y, 0.0f);
			gd.enabled_physics = true;
			gd.scale = 3.0f;


			b2BodyDef bd;
				bd.type = b2BodyType::b2_dynamicBody;
				bd.position = b2Vec2(nextItemPos.x, nextItemPos.y);
				bd.linearVelocity = nextItemSpeed;
				bd.fixedRotation = true;

			gd.objBodydef = &bd;

			b2FixtureDef fd;
				fd.isSensor = true;
				//b2CircleShape shape;
				//shape.m_radius = 1.0f;
				b2PolygonShape shape;
				shape.SetAsBox(gd.scale * gd.size.x / 2, gd.scale * gd.size.y / 2);
				fd.shape = &shape;

			gd.objFixDef = &fd;
			gd.texture = ResourceManager::GetTexturePtr(ResourceManager::lookupItemToTexture[currentItemType]);
			
			GameObject* obj = new ItemObject(ResourceManager::GetItemPtr(currentItemType), &gd);

			ObjectFactory::InsertObject(obj);

		}

		if (ImGui::TreeNode("Game Objects"))
		{
			ObjectFactory::Debug();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Game Feel"))
		{
			if (ImGui::SliderFloat3("G / mForce / m torque", &fricProp.x, 0.0f, 30.0f))
			{
				if(m_fricJoint)
					m_World->DestroyJoint(m_fricJoint);
				m_fricJoint = attachFrictionJoint2(b_bounds, m_player->getBody(), fricProp.x, fricProp.y, fricProp.z);
			}

			ImGui::TreePop();
		}

		m_player->Debug();
		m_astroidFactory->Debug();
	}

	void DrawBackGround() override
	{
		DrawString(5, 0, "Test Item functionality");
		DrawString(5, 15, "1.) pickup Various Items");
		DrawString(5, 30, "2.) Test items with Auto use (health and energy recovery)");
		DrawString(5, 45, "3.) Use Items based on selected item");
	}

	void DrawForeGround() override
	{
		ObjectFactory::DrawObjects();
	}

	static Game* Create() { return new ItemPickupScene; }
};

static int testIndex = RegisterGame("SpaceBuster", "Item Test", ItemPickupScene::Create);
