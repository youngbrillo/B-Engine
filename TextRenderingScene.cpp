#include "Game.h"
#include "imgui.h"
/// ------------------------------
#include "TextRenderer.h"
#include "UI_Button.h"
#include "Sprite.h"
#include "TextManager.h"
#include "imguiTextWrap.h"

const char* font_file_path = "./assets/fonts/DkHandRegular-orna.ttf";

struct AnimatingText
{

	glm::vec2	animTextPos = glm::vec2(351.50f, 25.0f),
				animationTimer = glm::vec2(0, 0.20f);
	bool animate = true, animationComplete = false;

	std::string textToDisplay = "I animate!", DisplayedText = "";
	int currentCharacter = 0;
	glm::vec4 _color = glm::vec4(0.936f, 0.892f, 0.181f, 1.0f);

	float scale = 0.7f;
	char buff[500];


	void onUpdate(float dt)
	{
		if (animationComplete)
		{
			//reset animate to true
			//reset characters in display text ? 
			currentCharacter = 0;
			animate = false;
			animationComplete = false;
			return;
		}
		if (animate)
		{
			animationTimer.x += dt;
			if (animationTimer.x >= animationTimer.y)
			{
				animationTimer.x = 0.0f;
				DisplayedText += textToDisplay[currentCharacter];
				currentCharacter++;
				if (currentCharacter >= textToDisplay.length())
				{
					animationComplete = true;
				}
			}
		}
	}
	void onDraw(TextRenderer& textRenderer)
	{
		textRenderer.DrawText(DisplayedText.c_str(), animTextPos.x, animTextPos.y, scale, _color);
	}
	void onDebug()
	{
		if (ImGui::InputText("String to write", &textToDisplay))
		{
		
		};



		ImGui::SliderFloat2("anim position", &animTextPos.x, -700, 700);
		ImGui::SliderFloat2("anim timer", &animationTimer.x, 0, 1.0f);
		if (ImGui::Checkbox("can animate", &animate))
		{
			if (animate)
			{
				DisplayedText = "";
			}
		}
		ImGui::Checkbox("animate complete", &animationComplete);
		ImGui::Separator();
	}
};
class TextRenderingScene : public Game
{
public:
	//constructor
	TextRenderer textRenderer;
	AnimatingText animTex;
	TextRenderingScene() : Game()
	{
		textRenderer = TextRenderer("./assets/fonts/OxygenMono-Regular.otf");
	//	textRenderer = TextRenderer("./assets/fonts/DkHandRegular-orna.ttf");
	//	textRenderer = TextRenderer("./assets/fonts/joystix monospace.otf");
		
		textRenderer.transform.position = glm::vec3(25.0f, 130.0f, 0.0f);
		textRenderer.transform.scale = 0.5f;
	};
	//destructor
	~TextRenderingScene()
	{

	};

	//key callbacks and frame input key callback
	void KeyboardUp(int key) override { }
	void KeyboardDown(int key)override { 
		float valid = false; int inc = 0;
		if (key == GLFW_KEY_TAB || key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT)
		{
			valid =  true;
			if (key == GLFW_KEY_LEFT)
				inc = -1;
			else
				inc = 1;
		}
		if(valid)
		{
			TextManager::IO().ShiftFont(inc);
		}

	}
	//mouse callbacks
	virtual void ResizeWindowCallback(GLFWwindow* window, int width, int height) override {
		Game::ResizeWindowCallback(window, width, height);

		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Game::AppCam->Width), 0.0f, static_cast<float>(Game::AppCam->Height));
		TextRenderer::fontShader->Use().SetMatrix4("projection", projection);
		printf("Scree Dimensions: (%d, %d)\n", Game::AppCam->Width, Game::AppCam->Height);
	}

	//frame updates
	void Update(float dt) override 
	{
		animTex.onUpdate(dt);
	}

	//collision callbacks 

	//screen drawing
	void DrawBackGround() override {}
	void DrawForeGround() override {}
	void DrawScene() override 
	{
		TextManager::IO().StartLine(position, scale);

		textRenderer.DrawText("(C)LearnOpenGL.com", 25.0f, 25.0f, scale, m_color);
		//TextManager::IO()->DrawText("This is sample text", glm::vec2(25.0f, 36.0f),scale, glm::vec4(0.5, 0.8f, 0.2f, 1.0f));
		//TextManager::IO()->DrawText("The quick brown fox jumped over the lazy dog", glm::vec2(25.0f, 36.0f + 72.0f/1.5f), scale, glm::vec4(0.9686f, 0.2824f, 0.2627f, 1.0f));

		//textRenderer.DrawText("Hello World");
		//textRenderer.DrawText(std::get<0>(fonts[fontIndex]).c_str(), AppCam->Width / 2.0f, AppCam->Height / 2.0f + 36.0f, scale);

		TextManager::IO().AddLine("Hello World, This is some sample text!!", m_color, true);
		TextManager::IO().AddLine("The quick brown fox jumped over the lazy dog", m_color, true);
		TextManager::IO().AddLine("1 2 3 4 5 6 7 8 9 0 - <>.,:;[]{} + = !@#$%^&*", m_color, true);
		TextManager::IO().AddLine("Using Font: ", m_color, false);
		TextManager::IO().AddLine(std::get<0>(TextManager::GetFonts()[TextManager::GetFontIndex()]).c_str(), alt_color);

		TextManager::IO().EndLine();


		animTex.onDraw(textRenderer);
	} 


	glm::vec2 position = glm::vec2(25.0f, AppCam->Height - 36.0f);
	float scale = 0.750f;
	glm::vec4 m_color = glm::vec4(1.0f), alt_color = glm::vec4(0.936f, 0.892f, 0.181f, 1.0f);
	int fontIndex = 0;

	void DrawDebug() override 
	{
		animTex.onDebug();

		ImGui::SliderFloat2("line position", &position.x, -AppCam->Height, AppCam->Height);
		ImGui::SliderFloat("line scaling", &scale, 0.01, 10.f);
		ImGui::ColorEdit4("main color", &m_color.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("alt color", &alt_color.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);

		const char* combo_preview_value =  std::get<0> (TextManager::GetFonts()[fontIndex]).c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Fonts", combo_preview_value))//, flags))
		{
			for (int n = 0; n < TextManager::GetFonts().size(); n++)
			{
				const bool is_selected = (fontIndex == n);
				if (ImGui::Selectable(std::get<0>(TextManager::GetFonts()[n]).c_str(), is_selected)) {
					fontIndex = n;
					textRenderer = TextRenderer(std::get<1>(TextManager::GetFonts()[fontIndex]).c_str());
					textRenderer.transform.position = glm::vec3(25.0f, 130.0f, 0.0f);
					textRenderer.transform.scale = 0.5f;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		
		
		textRenderer.Debug();
	}

	//creation
	static Game* Create() { return new TextRenderingScene; }
};

static int testIndex = RegisterGame("Test", "Text Rendering", TextRenderingScene::Create);  //do not actually register this lol
