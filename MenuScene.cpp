#include "Game.h"
#include "imgui.h"
#include "TextRenderer.h"
#include "Canvas.h"
#include "CanvasItem.h"
#include "ResourceManager.h"


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

		canvas.children.emplace_back(new CanvasText("Start Game"		,glm::vec2(50.0f, 165.0f)	, glm::vec4(1, 0, 0, 1.0f)));
		canvas.children.emplace_back(new CanvasText("Settings"			,glm::vec2(50.0f, 110.0f)	, glm::vec4(1, 1, 0, 1.0f)));
		canvas.children.emplace_back(new CanvasText("Credits"			,glm::vec2(50.0f, 055.0f)	, glm::vec4(1, 0, 1, 1.0f)));
		canvas.children.emplace_back(new CanvasText("Quit to Desktop"	,glm::vec2(50.0f, 000.0f)	, glm::vec4(0, 1, 1, 1.0f)));

		canvas.children.emplace_back(new CanvasText("Are you Sure?"		, glm::vec2(233.158, 250.158f), glm::vec4(0, 1, 1, 1.0f), false, false));
		canvas.children.emplace_back(new CanvasText("No wait!"					, glm::vec2(312.0f, 50.0f), glm::vec4(1.0f), false));
		canvas.children.emplace_back(new CanvasText("To the Desktop, posthaste!", glm::vec2(50.0f, -50.0f), glm::vec4(1.0f), false));


		canvas.t.position.y = Game::AppCam->Width / (canvas.children.size() + 1);
		canvas.t.pivot = glm::vec3(0.0f);
		canvas.t.UpdateMatrix();

		canvasShader = new Shader("canvas.vertex", "canvas.frag", true);

		localProjMatrix = glm::ortho(0.f,(float) AppCam->Width, 0.f, (float)AppCam->Height);
		canvasShader->Use().SetMatrix4("projection", localProjMatrix);
		
		
		{ //setup callbacks
			
			//canvas.children[3]->setSelectCallback(QuitScene);
			canvas.children[0]->func = new Callback(MenuScene::Wrapper_Transition_To_Scene, this); //start scene
			canvas.children[1]->func = new Callback(MenuScene::Wrapper_Transition_To_Scene, this); //view settings scene (todo: setup settings scene AKA pause menu)
			canvas.children[2]->func = new Callback(MenuScene::Wrapper_Transition_To_Scene, this); //view Credits ?
			canvas.children[3]->func = new Callback(MenuScene::Wrapper_HideMainMenuItems, this, false); //get confirm to quick
			//canvas.children[4]->func = new Callback(MenuScene::Wrapper_QuitScene, this); //no call back, this is just text haha.
			canvas.children[5]->func = new Callback(MenuScene::Wrapper_HideMainMenuItems, this, true); //restore to previous state
			canvas.children[6]->func = new Callback(MenuScene::Wrapper_QuitScene, this);//Quit to desktop
		}

	};

	static void Wrapper_Transition_To_Scene(MenuScene* i)
	{
		i->Transition_To_Scene();
	}
	void Transition_To_Scene()
	{
		printf("Start Scene: 'settings'!\n");
	}
	static void Wrapper_QuitScene(MenuScene* instance)
	{
		instance->QuitScene();
	}
	void QuitScene()
	{
		printf("Quiting Scene.\n");

		if (Game::mainWindow != nullptr) glfwSetWindowShouldClose(Game::mainWindow, GL_TRUE);
	}


	static void Wrapper_HideMainMenuItems(MenuScene* instance, bool makeVisible)
	{
		instance->HideMainMenuItems(makeVisible);
	}
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

	//key callbacks and frame input key callback
	void KeyboardUp(int key) override { }
	void KeyboardDown(int key)override { }
	

	//for a click to be valid it has to have been pressed and released in one go
	//the purpose of which is to ensure that a click can only be applied to one clickable item!
	bool validClick = false; 
	//int progress = 0;
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override
	{

		//if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
		//{
		//	progress++;
		//}
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) 
		{
			//progress++;
			validClick = true;
			for (auto child : canvas.children)
			{
				if (child->active && validClick) {
					child->handleCallback();
					validClick = false;
				};
			}
		//	progress = 0;
		}
	}
	virtual void mouseCallback_Cursor(GLFWwindow* window, double x, double y) override 
	{
		glm::vec2 mousePos = glm::vec2((float)x, (float) y);

		glm::mat4 projection = localProjMatrix;
		glm::mat4 view = glm::mat4(1.0f); //no view matrix
		glm::vec4 output;

		glm::vec4 NDC;

		//{ //get normalized device coords
			float w, h, xx, yy, ndc_x, ndc_y;
			w = AppCam->Width * 1.0f;
			h = AppCam->Height * 1.0f;
			xx = mousePos.x;
			yy = mousePos.y;



			ndc_x = 2.0 * xx/ w - 1.0;
			ndc_y = 1.0 - 2.0 * yy / h; //invert y axis

			NDC = glm::vec4(ndc_x, ndc_y, 0, 1);
		//}
		output = NDC * glm::inverse(projection);// *glm::inverse(view);

		//{
			//output.x += w / 2.0f;
			//output.y -= h / 2.0f;
		//}
		
		
		CanvasItem::mousePosition = glm::vec2(output.x + w/2.0f, output.y + h/2.0f);



	}
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override {}
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override {
		localProjMatrix = glm::ortho(0.f, (float)AppCam->Width, 0.f, (float)AppCam->Height);
		canvasShader->Use().SetMatrix4("projection", localProjMatrix);
		TextRenderer::fontShader->Use().SetMatrix4("projection", localProjMatrix);
		text.transform.position = glm::vec3(25.0f, AppCam->Height - 72.0f, 0.0f);

	}

	//frame updates
	void Update(float dt) override { canvas.Update(dt); }

	//screen drawing
	void DrawBackGround() override {}
	void DrawForeGround() override 
	{
		//text.DrawText("Start Game");
		canvas.Draw(canvasShader, m_surface);
		
	}
	//void DrawScene() override {} 
	bool useprojection2 = false;
	void DrawDebug() override 
	{
		ImGui::Checkbox("Proj. switch", &useprojection2);
		ImGui::SliderFloat2("Mouse pos", &CanvasItem::mousePosition.x, 0, 0);
		if (ImGui::Button("Hot Reload: Canvas Shader"))
		{
			delete canvasShader;
			canvasShader = new Shader("./canvas.vertex", "./canvas.frag", true);
			glm::mat4 _m_porj = glm::ortho(0.f, (float)AppCam->Width, 0.f, (float)AppCam->Height);
			if (useprojection2)
			{
				_m_porj = glm::ortho(0.f, (float)AppCam->Width, (float)AppCam->Height, 0.f);

			}
			canvasShader->Use().SetMatrix4("projection", _m_porj);

			printf("Reloaded Canvas Shader\n");
		}
		canvas.Debug("Main Menu Canvas");
		//text.Debug();

	}

	//creation
	static Game* Create() { return new MenuScene; }
};

static int testIndex = RegisterGame("Test", "MenuScene", MenuScene::Create);  
