#include "Game.h"

#include "imgui.h"

#include "Inventory.h"
#include "ItemObject.h"
#include "ResourceManager.h"
#include "Utility_Physics.h"
class InventoryScene : public Game
{
public:
	//members
	SpaceBuster::Inventory* m_inventory;
	Shader* ndcShader;

	InventoryScene() 
		: Game()
	{
		m_inventory = new SpaceBuster::Inventory();
		ResourceManager::startup();
		//m_Shader = ResourceManager::GetShaderPtr("SpriteShader");
		//m_Shader = ResourceManager::GetShaderPtr("ColorShader");
		m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);
		ndcShader = new Shader("NDC_sprite.vts", "stdsprite.frs", true);
		ndcShader->Use().setInt("image", 0);


		GameObject::g_GameObjectSurface = this->m_surface;
		GameObject::g_gameObjectShader = this->m_Shader;
	};
	~InventoryScene()
	{
		//printf(">\tDeconstructing Inventory scene\n");
		delete m_inventory;
		///m_Shader = nullptr;
		delete ndcShader;

		GameObject::g_GameObjectSurface = nullptr;
		GameObject::g_gameObjectShader = nullptr;

		ResourceManager::cleanup();
	};

	//key callbacks and frame input key callback
	void KeyboardUp(int key) override 
	{ 
		switch (key)
		{
		case GLFW_KEY_TAB:
			m_inventory->visible = !m_inventory->visible;
			break;
		default:
			break;
		}
	}
	void KeyboardDown(int key)override 
	{ 
		switch (key)
		{
		case GLFW_KEY_1:
			m_inventory->AddItem(ResourceManager::GetItemPtr(SpaceBuster::ItemType::SparePart));
			printf("Adding spare-part\n");
			break;
		case GLFW_KEY_2:
			m_inventory->AddItem(ResourceManager::GetItemPtr(SpaceBuster::ItemType::EnergyCell));
			printf("Adding energy-cell\n");
			break;
		case GLFW_KEY_3:
			m_inventory->AddItem(ResourceManager::GetItemPtr(SpaceBuster::ItemType::Ammo_reg));
			printf("Adding regular-ammunition\n");
			break;
		case GLFW_KEY_4:
			m_inventory->AddItem(ResourceManager::GetItemPtr(SpaceBuster::ItemType::Ammo_Reflect));
			printf("Adding reflective-ammunition\n");
			break;
		default:
			break;
		}
	}
	//mouse callbacks
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override 
	{
	
	}

	//frame updates
	void FixedUpdate(float dt) override { }
	void Update(float dt) override { }

	//screen drawing
	void DrawBackGround() override 
	{
	
		//m_inventory->Draw(ndcShader);
	}
	void DrawForeGround() override {
		DrawString(05, 0, "Inventory Scene");
		DrawString(05, 15, "Controls: Add to inventory");
		DrawString(35, 30, "1 - spare part");
		DrawString(35, 45, "2 - Energy cell");
		DrawString(35, 60, "3 - Ammo");
	}
	//void DrawScene() override {} 
	void DrawDebug() override {
		m_inventory->Debug();
	}

	//creation
	static Game* Create() { return new InventoryScene; }
};

//static int testIndex = RegisterGame("Scene", "Inventory Test", InventoryScene::Create);  //do not actually register this lol
