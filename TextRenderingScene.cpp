#include "Game.h"

#include "imgui.h"
#include "TextRenderer.h"
#include "UI_Button.h"


const char* font_file_path = "./assets/fonts/DkHandRegular-orna.ttf";
class TextRenderingScene : public Game
{
public:
	//constructor
	TextRenderer textRenderer;

	UIButton button;

	TextRenderingScene() : Game()
	{
		//textRenderer = TextRenderer();

	};
	//destructor
	~TextRenderingScene()
	{

	};

	//key callbacks and frame input key callback
	void KeyboardUp(int key) override { }
	void KeyboardDown(int key)override { }
	//mouse callbacks
	virtual void ResizeWindowCallback(GLFWwindow* window, int width, int height) override {
		Game::ResizeWindowCallback(window, width, height);

		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Game::AppCam->Width), 0.0f, static_cast<float>(Game::AppCam->Height));
		TextRenderer::fontShader->Use().SetMatrix4("projection", projection);
		printf("Scree Dimensions: (%d, %d)\n", Game::AppCam->Width, Game::AppCam->Height);
	}

	//frame updates
	void Update(float dt) override { }

	//collision callbacks 

	//screen drawing
	void DrawBackGround() override {}
	void DrawForeGround() override {}
	void DrawScene() override 
	{
		textRenderer.DrawText("Hello World");

		textRenderer.DrawText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec4(0.5, 0.8f, 0.2f, 1.0f));
		textRenderer.DrawText("(C) LearnOpenGL.com", AppCam->Width / 2.0f, AppCam->Height / 2.0f, 0.5f, glm::vec4(0.3, 0.7f, 0.9f, 1.0f));
		textRenderer.DrawText("The quick brown fox jumped over the lazy dog", 25.0f,25.0f + 36.f/1.5f, 1.0f, glm::vec4(0.9686f, 0.2824f, 0.2627f, 1.0f));


		button.Draw(textRenderer.fontShader, m_surface);
	} 
	void DrawDebug() override 
	{

		textRenderer.Debug();
		button.Debug();
	}

	//creation
	static Game* Create() { return new TextRenderingScene; }
};

static int testIndex = RegisterGame("Test", "Text Rendering", TextRenderingScene::Create);  //do not actually register this lol
