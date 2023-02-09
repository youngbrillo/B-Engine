#include "Game.h"
#include "imgui.h"
#include "TextRenderer.h"
#include "Canvas.h"
#include "CanvasItem.h"
#include "ResourceManager.h"
#include "App.h"


void ms_QuitScene()
{
	printf("MenuScene.cpp:: Quick Scene <class-less version>\n");
}
void printValues(int x, const char* string)
{
	printf("We have %d %s's left\n", x, string);
}
class MenuScene : public Game
{
public: //members
	TextRenderer text;
	Canvas canvas;
	Shader* canvasShader;
	glm::mat4 localProjMatrix;
public:
	//constructor
	MenuScene() : Game()
	{
		text = TextRenderer("./assets/fonts/joystix monospace.otf");

		text.transform.position = glm::vec3(25.0f, AppCam->Height - 72.0f, 0.0f);
		text.transform.scale = 0.5f;
		text.cachedDraw = true;

		CanvasItem::canvasText = &text; //set the canva's textrender reference 1st!

		canvas.children.emplace_back(new CanvasText("Start Game"		,glm::vec2(50.0f, 165.0f)	, glm::vec4(0, 1, 1, 1.0f)));
		canvas.children.emplace_back(new CanvasText("View Scores"			,glm::vec2(50.0f, 055.0f)	, glm::vec4(0, 1, 1, 1.0f)));
		canvas.children.emplace_back(new CanvasText("Demo"				,glm::vec2(50.0f, 110.0f)	, glm::vec4(0, 1.0, 1.0, 1.0f)));
		canvas.children.emplace_back(new CanvasText("Quit to Desktop"	,glm::vec2(50.0f, 000.0f)	, glm::vec4(1, 0, 0, 1.0f)));

		canvas.children.emplace_back(new CanvasText("Are you Sure?"		, glm::vec2(233.158, 250.158f), glm::vec4(1.0f, 0,0, 1.0f), false, false));
		canvas.children.emplace_back(new CanvasText("No wait!"					, glm::vec2(312.0f, 50.0f), glm::vec4(0.0f), false));
		canvas.children.emplace_back(new CanvasText("To the Desktop, posthaste!", glm::vec2(50.0f, -50.0f), glm::vec4(0.0f), false));


		canvas.t.position.y = Game::AppCam->Width / (canvas.children.size() + 1);
		canvas.t.pivot = glm::vec3(0.0f);
		canvas.t.UpdateMatrix();

		canvasShader = new Shader("canvas.vertex", "canvas.frag", true);

		localProjMatrix = glm::ortho(0.f,(float) AppCam->Width, 0.f, (float)AppCam->Height);
		canvasShader->Use().SetMatrix4("projection", localProjMatrix);
		
		
		{ //setup callbacks
			
			//canvas.children[3]->setSelectCallback(QuitScene);
			canvas.children[0]->func = new Callback(MenuScene::Wrapper_Transition_To_Scene, this, "Final", "SpaceBuster III"); //start scene
			canvas.children[1]->func = new Callback(MenuScene::Wrapper_Transition_To_Scene, this, "Final", "viewScores"); //view settings scene (todo: setup settings scene AKA pause menu)
			canvas.children[2]->func = new Callback(MenuScene::Wrapper_Transition_To_Scene, this, "Final", "GameSetup"); //view Credits ?
			canvas.children[3]->func = new Callback(MenuScene::Wrapper_HideMainMenuItems, this, false); //get confirm to quick
			//canvas.children[4]->func = new Callback(MenuScene::Wrapper_QuitScene, this); //no call back, this is just text haha.
			canvas.children[5]->func = new Callback(MenuScene::Wrapper_HideMainMenuItems, this, true); //restore to previous state
			canvas.children[6]->func = new Callback(MenuScene::Wrapper_QuitScene, this);//Quit to desktop
		}

	};

	static void Wrapper_Transition_To_Scene(MenuScene* i, const char* categoryName, const char* sceneName) { i->Transition_To_Scene(categoryName,sceneName); }
	void Transition_To_Scene(const char* categoryName, const char* sceneName)
	{
		printf("Start Scene: '%s/%s'!\n", categoryName, sceneName);
		int k = App::GetScene(categoryName, sceneName);
		if (k == -1)
		{
			printf("Unable to load scene %d '%s/%s\n",k, categoryName, sceneName);
		}
		else
		{
			App::LoadScene(k);
		}
	}
	static void Wrapper_QuitScene(MenuScene* instance) { instance->QuitScene(); }
	void QuitScene() { printf("Quiting Scene.\n"); if (Game::mainWindow != nullptr) { glfwSetWindowShouldClose(Game::mainWindow, GL_TRUE); } }

	static void Wrapper_HideMainMenuItems(MenuScene* instance, bool makeVisible) { instance->HideMainMenuItems(makeVisible); }
	void HideMainMenuItems(bool makeVisible)
	{
		canvas.children[0]->visible = makeVisible;
		canvas.children[1]->visible = makeVisible;
		canvas.children[2]->visible = makeVisible;
		canvas.children[3]->visible = makeVisible;

		canvas.children[4]->visible = !makeVisible;
		canvas.children[5]->visible = !makeVisible;
		canvas.children[6]->visible = !makeVisible;
	}




	//destructor
	~MenuScene()
	{
		delete canvasShader;
		CanvasItem::canvasText = nullptr;
	};

	//key callbacks and frame input key callback, //unfortunately keyboard navigation is out of scope :*(

	//for a click to be valid it has to have been pressed and released in one go
	//the purpose of which is to ensure that a click can only be applied to one clickable item!
	bool validClick = false; 
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override
	{
		if (ImGui::GetIO().WantCaptureMouse) return;
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) 
		{
			validClick = true;
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			glm::vec2 mousePos = glm::vec2((float)x, (float)y);
			mousePos = ScreenToWorldCoords_projectionMatOnly(localProjMatrix, mousePos);

			for (int i = 0; i < canvas.children.size(); i++)
			{
				//a bit ineffecient, but at least it doesn't run every frame
				if (canvas.children[i]->active && validClick && canvas.children[i]->isPointInBounds(mousePos, &canvas.t))
				{
					canvas.children[i]->handleCallback();
					validClick = false;
					return;
				}
				else if (App::g_App->state == App::App_State::state_paused)
				{//then...
					if (canvas.children[i]->isPointInBounds(mousePos, &canvas.t))
					{
						canvas.children[i]->handleCallback();
					}
				}
			}
		}
	}

	glm::vec2 ScreenToWorldCoords_projectionMatOnly(const glm::mat4& inputMat, const glm::vec2& pos)
	{

		glm::vec2 mousePos = glm::vec2(pos.x, pos.y);

		glm::mat4 projection = inputMat;
		glm::vec4 output;
		glm::vec4 NDC;

		float w, h, xx, yy, ndc_x, ndc_y;
		w = AppCam->Width * 1.0f;
		h = AppCam->Height * 1.0f;
		xx = mousePos.x;
		yy = mousePos.y;


		ndc_x = 2.0 * xx / w - 1.0;
		ndc_y = 1.0 - 2.0 * yy / h; //invert y axis

		NDC = glm::vec4(ndc_x, ndc_y, 0, 1);
		output = NDC * glm::inverse(projection);// *glm::inverse(view);
		return glm::vec2(output.x + w / 2.0f, output.y + h / 2.0f);
	}

	virtual void mouseCallback_Cursor(GLFWwindow* window, double x, double y) override 
	{
		glm::vec2 mousePos = glm::vec2((float)x, (float) y);

		CanvasItem::mousePosition = ScreenToWorldCoords_projectionMatOnly(localProjMatrix, mousePos);

		if (App::g_App->state == App::App_State::state_paused)
		{
			for (int i = 0; i < canvas.children.size(); i++)
			{
				if (canvas.children[i]->isPointInBounds(CanvasItem::mousePosition, &canvas.t))
				{
					canvas.children[i]->active = true;
				}
				else
				{
					canvas.children[i]->active = false;
				}
			}
		}
	}
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override {
		localProjMatrix = glm::ortho(0.f, (float)AppCam->Width, 0.f, (float)AppCam->Height);
		//set matrix positions
		canvasShader->Use().SetMatrix4("projection", localProjMatrix);
		TextRenderer::fontShader->Use().SetMatrix4("projection", localProjMatrix);
		//stabalize text 
		text.transform.position = glm::vec3(25.0f, AppCam->Height - 72.0f, 0.0f);

	}

	void Update(float dt) override { canvas.Update(dt); }

	void DrawScene() override 
	{
		canvas.Draw(canvasShader, m_surface);
	} 
	void DrawDebug() override 
	{
		ImGui::SliderFloat2("Mouse pos", &CanvasItem::mousePosition.x, 0, 0);
		if (ImGui::Button("Hot Reload: Canvas Shader"))
		{
			delete canvasShader;
			canvasShader = new Shader("./canvas.vertex", "./canvas.frag", true);
			localProjMatrix = glm::ortho(0.f, (float)AppCam->Width, 0.f, (float)AppCam->Height);

			TextRenderer::fontShader->Use().SetMatrix4("projection", localProjMatrix);
			canvasShader->Use().SetMatrix4("projection", localProjMatrix);

			printf("Reloaded Canvas Shader\n");
		}
		canvas.Debug("Main Menu Canvas");
		//text.Debug();

	}

	//creation
	static Game* Create() { return new MenuScene; }
};

static int testIndex = RegisterGame("Final", "MenuScene", MenuScene::Create);  
