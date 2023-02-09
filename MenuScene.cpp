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
		CanvasItem::canvasText = &text;

		printf("bound x value should be '5'\n");
		canvas.children.emplace_back(new CanvasText("Start Game"		,glm::vec2(50.0f, 000.0f)	, glm::vec4(1, 0, 0, 1.0f)));
		canvas.children.emplace_back(new CanvasText("Settings"			,glm::vec2(50.0f, 055.0f)	, glm::vec4(1, 1, 0, 1.0f)));
		canvas.children.emplace_back(new CanvasText("Credits"			,glm::vec2(50.0f, 110.0f)	, glm::vec4(1, 0, 1, 1.0f)));
		canvas.children.emplace_back(new CanvasText("Quit to Desktop"	,glm::vec2(50.0f, 165.0f)	, glm::vec4(0, 1, 1, 1.0f)));
		canvas.t.position.y = Game::AppCam->Width / (canvas.children.size() + 1);
		canvas.t.pivot = glm::vec3(0.0f);
		canvas.t.UpdateMatrix();

		//canvas.children.emplace_back(new CanvasItem(ResourceManager::GetTexturePtr("default"), glm::vec2(0.0f),  glm::vec4(1, 0, 0, 1.0f)));
		//canvas.children.emplace_back(new CanvasItem(ResourceManager::GetTexturePtr("default"), glm::vec2(0.0f, 55.0f),  glm::vec4(1, 1, 0, 1.0f)));
		//canvas.children.emplace_back(new CanvasItem(ResourceManager::GetTexturePtr("default"), glm::vec2(0.0f, 110.0f),  glm::vec4(1, 0, 1, 1.0f)));
		//canvas.children.emplace_back(new CanvasItem(ResourceManager::GetTexturePtr("default"), glm::vec2(0.0f, 165.0f),  glm::vec4(0, 1, 1, 1.0f)));
		canvasShader = new Shader("canvas.vertex", "canvas.frag", true);

		//glm::mat4 projection = glm::ortho(0.f, (float)AppCam->Width, (float)AppCam->Height, 0.f);
		localProjMatrix = glm::ortho(0.f,(float) AppCam->Width, 0.f, (float)AppCam->Height);
		canvasShader->Use().SetMatrix4("projection", localProjMatrix);

		//canvas.children[3]->setSelectCallback(QuitScene);
		canvas.children[3]->func = new Callback(MenuScene::Wrapper_QuitScene, this);

	};

	static void Wrapper_QuitScene(MenuScene* instance)
	{
		instance->QuitScene();
	}
	void QuitScene()
	{
		printf("did it work?\tWe are about to see..\n");

		if (Game::mainWindow != nullptr)
			glfwSetWindowShouldClose(Game::mainWindow, GL_TRUE);

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

	//mouse callbacks - i will need these for the UI Buttons
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override
	{
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
		{
			for (auto child : canvas.children)
			{
				if (child->active) child->handleCallback();
			}
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
