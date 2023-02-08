#include "Game.h"
#include "imgui.h"
#include "TextRenderer.h"
#include "Canvas.h"
#include "CanvasItem.h"
#include "ResourceManager.h"
class MenuScene : public Game
{
public: //members
	TextRenderer text;
	Canvas canvas;
	Shader* canvasShader;
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
		glm::mat4 _m_porj = glm::ortho(0.f,(float) AppCam->Width, 0.f, (float)AppCam->Height);
		canvasShader->Use().SetMatrix4("projection", _m_porj);

	};
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
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override {}
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) override 
	{
		glm::vec2 screenPosition = glm::vec2((float)x, (float) y);

		CanvasItem::mousePosition = screenPosition;
	}
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override {}
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override {
		glm::mat4 _m_porj = glm::ortho(0.f, (float)AppCam->Width, 0.f, (float)AppCam->Height);
		canvasShader->Use().SetMatrix4("projection", _m_porj);
		TextRenderer::fontShader->Use().SetMatrix4("projection", _m_porj);
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
