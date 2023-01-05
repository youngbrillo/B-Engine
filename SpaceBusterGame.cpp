#include "SpaceBusterGame.h"
#include "ObjectFactory.h"

using namespace SpaceBuster;


SpaceBusterGame::SpaceBusterGame()
	: Game()
	
{
	//nullify gravity
	m_World->SetGravity(b2Vec2(0.0f, 0.0f));

	{//setup ground
		m_groundBody = generateBounds_rect(m_World, b2Vec2(0.0f, 20.0f), 90.0f, 40.0f, 0.40f); 
	}
	{//setup player
		m_player = new ShipController(m_World, b2Vec2_zero, m_groundBody);
	}
	//setup camera
	m_cameraController = new CameraController(Game::AppCam, m_player->getBody());
	m_cameraController->mode = CameraController::FollowMode::Instant;

	//setup factory
	m_astroidFactory = new  AstroidFactory(1);
	m_astroidFactory->autoGenerateEnabled = true;

	//setup shaders
	m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);

	//assign globals
	GameObject::g_GameObjectSurface = this->m_surface;
	GameObject::g_gameObjectShader = this->m_Shader;
	ShipController::g_shipController = m_player;

}

SpaceBusterGame::~SpaceBusterGame()
{
	delete m_player;
	delete m_astroidFactory;
	delete m_cameraController;
	ObjectFactory::cleanup();

	
	GameObject::g_GameObjectSurface = nullptr;
	GameObject::g_gameObjectShader = nullptr;
	ShipController::g_shipController = nullptr;
}

void SpaceBusterGame::FixedUpdate(float deltaTime)
{
	Game::FixedUpdate(deltaTime);
	m_player->FixedUpdate(deltaTime);
}

void SpaceBusterGame::Update(float deltaTime)
{
	Game::Update(deltaTime);
	m_player->Update(deltaTime);
	m_astroidFactory->Update(deltaTime);
	m_cameraController->Update(deltaTime);
}

//physic listeners
void SpaceBusterGame::onBeginContact(b2Contact* contact)
{
	Game::onBeginContact(contact);
	m_player->handleBeginContact(contact);
}

void SpaceBusterGame::onEndContact(b2Contact* contact)
{
	Game::onEndContact(contact);
	m_player->handleBeginContact(contact);
}

void SpaceBusterGame::onPreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	Game::onPreSolve(contact, oldManifold);
	m_player->handlePreSolve(contact, oldManifold);
}

void SpaceBusterGame::onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	Game::onPostSolve(contact, impulse);
	m_player->handlePostSolve(contact, impulse);
}

//keyboard inputs
void SpaceBusterGame::KeyboardUp(int key)
{
	if(m_player)
		m_player->onKeyRelease(key);
}

void SpaceBusterGame::KeyboardDown(int key)
{
	if (m_player)
		m_player->onKeyPress(key);
}

void SpaceBusterGame::DrawDebug()
{
	m_player->Debug();
	m_cameraController->Debug();
	m_astroidFactory->Debug();
	ObjectFactory::Debug();
}


//draw calls
void SpaceBusterGame::DrawScene()
{
	Game::DrawScene();
}

void SpaceBusterGame::DrawForeGround()
{
	ObjectFactory::DrawObjects();
}

void SpaceBusterGame::DrawBackGround()
{
}
