#include "Game.h"
#include "imgui.h"

#include "Canvas.h"
#include "CanvasItem.h"

#include "FileIO.h"
#include "FileData.h"
#include "TextRenderer.h"
#include "App.h"
class ScoreScene : public Game
{
public:
	//constructor
	TextRenderer text;
	Canvas canvas;
	Shader* canvasShader;
	glm::mat4 localProjMatrix;

	FileIO::ScoreFile* m_scores = nullptr;
	float canvasTextSize = 0.50f;
	glm::vec2 startingPosition = glm::vec2(150.0f, 450.0f);




	void ReconfigureScores()
	{
		canvas = Canvas();

		glm::vec2 startingPos, offset;
		startingPos = startingPosition;
		offset = glm::vec2(0.0f, -60.0f *  canvasTextSize);

		for (int i = 0; i < m_scores->scores.size(); i++)
		{
			//printf("\t%d\n", i);
			std::string score = "";
			//score += std::to_string(m_scores->scores[i]);
			char buffer[17]; sprintf_s(buffer, "%010d", m_scores->scores[i]);
			score += buffer;
			glm::vec4 _col = glm::vec4(0, 1.0, 1.0, 1.0f);
			if (i == 0) 
			{
				score += " <- Hi Score!!!";
				_col = glm::vec4(0.936f, 0.936f, 0.161f, 1.0f);
			}
			
			CanvasText* t = new CanvasText(score, startingPos, _col, true, false);
			t->transform.scale = canvasTextSize;
			canvas.children.emplace_back(t);

			startingPos += offset;
		}

		CanvasItem* back_button = new CanvasText("Return", glm::vec2(25.0f, 514), glm::vec4(0, 1.0, 1.0, 1.0f));
		back_button->transform.scale = 1.0f;
		back_button->func = new Callback(ScoreScene::Wrapper_Transition_To_Scene, this, "Final", "MenuScene");

		canvas.children.emplace_back(back_button);
		//
	}


	static void Wrapper_Transition_To_Scene(ScoreScene* i, const char* categoryName, const char* sceneName) { i->Transition_To_Scene(categoryName, sceneName); }
	void Transition_To_Scene(const char* categoryName, const char* sceneName)
	{
		printf("Start Scene: '%s/%s'!\n", categoryName, sceneName);
		int k = App::GetScene(categoryName, sceneName);
		if (k == -1)
		{
			printf("Unable to load scene %d '%s/%s\n", k, categoryName, sceneName);
		}
		else
		{
			App::LoadScene(k);
		}
	}


	ScoreScene() : Game()
	{
		text = TextRenderer("./assets/fonts/joystix monospace.otf");

		text.transform.position = glm::vec3(25.0f, AppCam->Height - 72.0f, 0.0f);
		text.transform.scale = 0.5f;
		text.cachedDraw = true;
		CanvasItem::canvasText = &text; //set the canva's textrender reference 1st!

		//load scores
		m_scores = new FileIO::ScoreFile("./assets/Scores.json");
		printf("hi score: %d\n", m_scores->highestScore);
		
		ReconfigureScores();
		//try not to reconfigure it again please :)


		canvasShader = new Shader("canvas.vertex", "canvas.frag", true);

		localProjMatrix = glm::ortho(0.f, (float)AppCam->Width, 0.f, (float)AppCam->Height);
		canvasShader->Use().SetMatrix4("projection", localProjMatrix);

	};
	//destructor
	~ScoreScene()
	{
		delete canvasShader;
		CanvasItem::canvasText = nullptr;
	};

	//key callbacks and frame input key callback
	void KeyboardUp(int key) override { }
	void KeyboardDown(int key)override { }
	virtual void processInput(GLFWwindow* window, float deltaTime) override { }
	//mouse callbacks

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

	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override
	{

		if (ImGui::GetIO().WantCaptureMouse) return;
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			glm::vec2 mousePos = glm::vec2((float)x, (float)y);
			mousePos = ScreenToWorldCoords_projectionMatOnly(localProjMatrix, mousePos);

			for (int i = 0; i < canvas.children.size(); i++)
			{
				//a bit ineffecient, but at least it doesn't run every frame
				if (canvas.children[i]->active  && canvas.children[i]->isPointInBounds(mousePos, &canvas.t))
				{
					canvas.children[i]->handleCallback();
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
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) override 
	{

		glm::vec2 mousePos = glm::vec2((float)x, (float)y);

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
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override 
	{
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
		if (ImGui::SliderFloat2("Text Pos", &startingPosition.x,-AppCam->Width, AppCam->Width))
		{
			ReconfigureScores();
		}
		if (ImGui::SliderFloat("Text size", &canvasTextSize, 0.01, 1.0f))
		{
			ReconfigureScores();
		}
		canvas.Debug("Score scene Canvas");
	}

	//creation
	static Game* Create() { return new ScoreScene; }
};

static int testIndex = RegisterGame("Final", "viewScores", ScoreScene::Create);  //do not actually register this lol
