#include "App.h"
#include "ResourceManager.h"

App* App::g_App = nullptr;
Settings App::g_settings = Settings();

App::App(unsigned int width, unsigned int height)
	: CSApp(width, height)
	, m_game(nullptr)
	, m_Camera(nullptr)
{
	m_Camera = new Camera(glm::vec4(0, 0, width, height));
	Game::AppCam = m_Camera;
	g_App = this;
}

App::~App()
{
	Game::AppCam = nullptr;
	delete m_Camera;
	delete m_game;
	ResourceManager::cleanup();
	g_App = nullptr;
}

void App::Init()
{
	orderGameEntries();
	//m_gameSelection = 6;
	ResourceManager::startup();
	m_game = g_GameEntries[g_settings.m_index].creationFunc();
}

void App::ProcessInput(GLFWwindow* win, float deltaTime)
{
	//set up keys later
}

void App::FixedUpdate(float deltaTime)
{
	if (Game::m_World != nullptr) 
	{
		float timestep = 1 / 60.0f;

		int velocityIterations = 8, positionIterations = 3;
		if(m_game) 
			m_game->FixedUpdate(deltaTime);
		Game::m_World->Step(timestep, velocityIterations, positionIterations);
	}
}

void App::Update(float deltaTime)
{
	if (state == App_State::state_paused) return;
	timeSinceLastFrame += deltaTime;
	if (timeSinceLastFrame >= timeSinceLastFrame_Target)
	{
		timeSinceLastFrame = 0.0f;
		FixedUpdate(deltaTime);
	}

	if (m_game != nullptr)
	{
		m_game->Update(deltaTime);
	}
}

void App::Render()
{

	if (m_game != nullptr)
	{
		m_game->DrawScene();
	}
	DebugRender();
}

void App::DebugRender()
{

	{
		ImGui::Begin("Code Switch Debug");//, & showUI, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		CSApp::DebugMenu();


		if (ImGui::BeginTabBar("ControlTabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Controls"))
			{

				ImGui::Separator();

				ImVec2 button_sz = ImVec2(-1, 0);
				if (ImGui::Button("Pause (P)", button_sz))
				{
					state = state == App_State::state_paused ? App_State::state_running : App_State::state_paused;
				}
				if (state == App_State::state_paused)
				{
					if (ImGui::Button("Step (Ctrl + R Arrow)", button_sz))
					{
						if (Game::m_World != nullptr)
						{
							float timestep = 1 / 60.0f;

							int velocityIterations = 8, positionIterations = 3;
							Game::m_World->Step(timestep, velocityIterations, positionIterations);
						}
					}
				}

				if (ImGui::Button("Restart (R)", button_sz))
				{
					restartGame();
				}

				if (ImGui::Button("Quit", button_sz))
				{
					if(mainWindow != nullptr)
						glfwSetWindowShouldClose(mainWindow, GL_TRUE);
				}

				ImGui::Separator();

				if (m_game) {
					m_game->DrawDebug();
				}

				ImGui::EndTabItem();
			}

			m_Camera->debug();
			RenderDifferentApp();
			ImGui::EndTabBar();
		}

		ImGui::End();

	}
}

void App::RenderDifferentApp()
{
	ImGuiTreeNodeFlags leafNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	leafNodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (ImGui::BeginTabItem("Apps"))
	{
		int categoryIndex = 0;
		const char* category = g_GameEntries[categoryIndex].category;
		int i = 0;
		while (i < g_GameCount)
		{
			bool categorySelected = strcmp(category, g_GameEntries[g_settings.m_index].category) == 0;
			ImGuiTreeNodeFlags nodeSelectionFlags = categorySelected ? ImGuiTreeNodeFlags_Selected : 0;
			bool nodeOpen = ImGui::TreeNodeEx(category, nodeFlags | nodeSelectionFlags);

			if (nodeOpen)
			{
				while (i < g_GameCount && strcmp(category, g_GameEntries[i].category) == 0)
				{
					ImGuiTreeNodeFlags selectionFlags = 0;
					if (g_settings.m_index == i)
					{
						selectionFlags = ImGuiTreeNodeFlags_Selected;
					}
					ImGui::TreeNodeEx((void*)(intptr_t)i, leafNodeFlags | selectionFlags, "%s", g_GameEntries[i].name);
					if (ImGui::IsItemClicked())
					{
						g_settings.m_index = i;
						restartGame();
					}
					++i;
				}
				ImGui::TreePop();
			}
			else
			{
				while (i < g_GameCount && strcmp(category, g_GameEntries[i].category) == 0)
				{
					++i;
				}
			}

			if (i < g_GameCount)
			{
				category = g_GameEntries[i].category;
				categoryIndex = i;
			}
		}
		ImGui::EndTabItem();
	}
}

void App::restartGame()
{
	printf("Restarting Game: %d. %s\n", g_settings.m_index, g_GameEntries[g_settings.m_index].name);
	delete m_game;
	m_Camera->Reset();
	m_game = g_GameEntries[g_settings.m_index].creationFunc();
}
void App::orderGameEntries()
{
	//sort in Alpha-beta by name
	std::sort(g_GameEntries, g_GameEntries + g_GameCount, [](const GameEntry& lhs, const GameEntry& rhs)
		{return lhs.name < rhs.name; });
	//sort in Alpha-beta by category
	std::sort(g_GameEntries, g_GameEntries + g_GameCount, [](const GameEntry& lhs, const GameEntry& rhs)
		{return lhs.category < rhs.category; });

}

void App::KeyCallBack(GLFWwindow* win, int key, int scancode, int action, int mode)
{

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{

	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		this->state = state == App_State::state_running ? App_State::state_paused : App_State::state_running;
	}

	if (!m_game) return;

	if (action == GLFW_PRESS)
	{
		m_game->KeyboardDown(key);
	}

	if (action == GLFW_RELEASE)
	{
		m_game->KeyboardUp(key);
	}
	if (action == GLFW_REPEAT)
	{
		m_game->Keyboard(key);
	}
}

void App::mouseCallback(GLFWwindow*window, int button, int action, int mode)
{
	m_Camera->mouseCallback(window, button, action, mode);
	m_game->mouseCallback(window, button, action, mode);
}

void App::mouseCallback_Cursor(GLFWwindow* w, double x, double y)
{
	m_Camera->mouseCallback_Cursor(w, x, y);
	m_game->mouseCallback_Cursor(w, x, y);
}

void App::mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_Camera->mouseScroll_callback(window, xoffset, yoffset);
	m_game->mouseScroll_callback(window, xoffset, yoffset);
}

void App::ResizeWindowCallback(GLFWwindow* window, int width, int height)
{
	screenWidth = width;
	screenHeight = height;

	m_Camera->Width = width;
	m_Camera->Height = height;

	m_Camera->updateCamVectors();
	m_game->ResizeWindowCallback(window, width, height);

}
