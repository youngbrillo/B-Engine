#include "CSApp.h"
#include "Game.h"
GLFWwindow* CSApp::mainWindow = nullptr;

CSApp::CSApp(unsigned int width, unsigned int height) :
	  state (App_State::state_running)
	, Keys()
	, screenWidth(width), screenHeight(height)
	, ImGUI_Demo_visible(false), showUI(true)
	, timeSinceLastFrame(0)
	, timeSinceLastFrame_Target(1.0f/60.0f)
{
}

void CSApp::setKey(int key, bool value)
{
	this->Keys[key] = value;
}

void CSApp::DebugMenu()
{

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Show The Demo Window", &ImGUI_Demo_visible);
	ImGui::ColorEdit4("Window background Color", &Game::windowColorBackGround.x);

	static int elem = App_State::state_paused; //b/c its first
	const char* elem_names[App_State::state_running + 1] = { "PAUSED", "Running" };
	const char* elem_name = (elem >= 0 && elem < App_State::state_running + 1) ? elem_names[elem] : "Unknown";

	elem = this->state;
	if (ImGui::SliderInt("State", &elem, 0, App_State::state_running, elem_name)) {
		this->state = (App_State)elem;
	}
	if (ImGui::TreeNode("Fixed Update intervals"))
	{
		ImGui::SliderFloat("TSLF (time since last frame)", &timeSinceLastFrame, 0, 1);
		ImGui::SliderFloat("TSLF Target", &timeSinceLastFrame_Target, 0, 1);
		ImGui::TreePop();
	}

	if (ImGUI_Demo_visible)
		ImGui::ShowDemoWindow(&ImGUI_Demo_visible);

	ImGui::Separator();
}