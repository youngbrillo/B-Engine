#include "Game.h"
#include "baseContactListener.h"
#include "ObjectFactory.h"

GameEntry g_GameEntries[MAX_APPS] = { {nullptr} };
int g_GameCount = 0;

int RegisterGame(const char* category, const char* name, Game_CreateFunction* func)
{
	int index = g_GameCount;
	if (index < MAX_APPS)
	{
		g_GameEntries[index] = { category, name, func };
		++g_GameCount;
		return index;
	}

	return -1;
}


b2World* Game::m_World = nullptr;
Camera* Game::AppCam = nullptr;
GLFWwindow* Game::mainWindow = nullptr;
glm::vec4 Game::windowColorBackGround = glm::vec4(0.310f, 0.310f, 0.310f, 1.0f);

Game::Game()
	: m_groundBody(nullptr)
	, m_Shader(nullptr)
	, m_surface(nullptr)
	, m_physDrawer(nullptr)
{
	//printf(">\tConstructing Game\n");
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_World = new b2World(gravity);


	m_Shader = new Shader("test.vts", "test.frs", true);
	m_surface = new quadSurface(); m_surface->Generate();
	m_physDrawer = new PhysicsDebugger();
	m_physDrawer->Create(m_Shader);
	m_World->SetDebugDraw(m_physDrawer);

	m_World->SetContactListener(new baseContactListener(this));
	Game::windowColorBackGround = glm::vec4(0.310f, 0.310f, 0.310f, 1.0f);

}  

Game::~Game()
{
	//printf(">\tDeconstructing Game\n");
	if (m_groundBody != nullptr) {
		m_World->DestroyBody(m_groundBody);
		m_groundBody = nullptr;
	}
	ObjectFactory::cleanup();

	delete m_World;
	delete m_Shader;
	delete m_surface;
	delete m_physDrawer;
}

void Game::DrawTitle(const char* string)
{
}

void Game::DrawScene()
{
	//printf("game, shader value %d\n", m_Shader->GetId());

	glm::mat4 model = glm::mat4(1);
	glm::mat4 projection = AppCam->getProjectionMatrix();
	glm::mat4 view = AppCam->GetViewMatrix();


	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::scale(model, glm::vec3(1) * 0.5f);

	m_Shader->Use()
		.SetMatrix4("projection", projection)
		.SetMatrix4("view", view);

	DrawBackGround();

	m_Shader->Use()
		.SetMatrix4("model", model)
		.SetVector4f("color", glm::vec4(1,0.5, 1, 1));

	m_surface->Bind();


	DrawForeGround();

	m_World->DrawDebugData();
	m_physDrawer->SetFlags_Internal();
	m_physDrawer->setMat4(projection * view);
	m_physDrawer->Flush();
}

void Game::DrawDebug()
{
}

void Game::processInput(GLFWwindow* window, float deltaTime)
{

}

void Game::FixedUpdate(float deltaTime)
{
	ObjectFactory::FixedUpdateObjects(deltaTime);
}

void Game::Update(float deltaTime)
{
	ObjectFactory::UpdateObjects(deltaTime);
}

void Game::onBeginContact(b2Contact* contact)
{
	ObjectFactory::handleBeginContact(contact);
}

void Game::onEndContact(b2Contact* contact)
{
	ObjectFactory::handleEndContact(contact);
}

void Game::onPreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	ObjectFactory::handlePreSolve(contact, oldManifold);
}

void Game::onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	ObjectFactory::handlePostSolve(contact, impulse);
}
