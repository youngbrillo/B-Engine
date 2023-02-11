#include "Game.h"
#include <imgui.h>

#include "NeoShipController.h"
#include "AstroidManager.h"
#include "ObjectFactory.h"
#include "UI_Button.h"
#include "ResourceManager.h"
#include "Canvas.h"
#include "CanvasItem.h"
#include "TextRenderer.h"

#include "FileIO.h"
#include "FileData.h"
#include "App.h"

#include "MissionSurvival.h"

class SpaceBusterIIISetup : public Game
{
public:
	//members
	NeoShipController* shipController;
	SpaceBuster::AstroidFactory* astroidFactory;
	Canvas uiCanvas;
	TextRenderer m_textRenderer;
	Shader* CanvasShader;

	FileIO::ScoreFile* m_scores = nullptr;
	glm::mat4 canvasProjectionMatrix;

	MissionSurvival* contrainAstroidsToscreen;
	CanvasItem* back_button, *restart_button;

	glm::vec3 hiScorePosition, elpasedTimePosition;
	std::string hiScoreText;
public:
	SpaceBusterIIISetup() : Game()
	{
		//setup helpers & managers
		CanvasItem::mousePosition = glm::vec2(0.0f);

		m_World->SetGravity(b2Vec2(0, 0));
		astroidFactory = new SpaceBuster::AstroidFactory(5U);

		//initalize shaders
		m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);
		CanvasShader = new Shader("canvas.vertex", "canvas.frag", true);

		//setup game objects
		b2BodyDef bd;
		{
			bd.position = b2Vec2(0.0f, -20.0f);
			bd.type = b2BodyType::b2_staticBody;
			m_groundBody = m_World->CreateBody(&bd); //empty static body to anchor to
			//float extents = 400;
			//{
			//	b2EdgeShape shape;
			//	//shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
			//	shape.Set(b2Vec2(-extents, 0.0f), b2Vec2(extents, 0.0f));
			//	m_groundBody->CreateFixture(&shape, 0.0f);

			//	//shape.Set(b2Vec2( 40.0f, 40.0f), b2Vec2(40.0f, 0.0f));
			//	shape.Set(b2Vec2(extents, extents), b2Vec2(extents, 0.0f));
			//	m_groundBody->CreateFixture(&shape, 0.0f);

			//	//shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(-40.0f, 40.0f));
			//	shape.Set(b2Vec2(-extents, 0.0f), b2Vec2(-extents, extents));
			//	m_groundBody->CreateFixture(&shape, 0.0f);

			//	//shape.Set(b2Vec2(-40.0f, 40.0f), b2Vec2(40.0f, 40.0f));
			//	shape.Set(b2Vec2(-extents, extents), b2Vec2(extents, extents));
			//	m_groundBody->CreateFixture(&shape, 0.0f);
			//}


		}
		NeoShipControllerDefinition def;
		shipController = new NeoShipController(&def, m_groundBody);


		//setup UI
		m_textRenderer = TextRenderer("./assets/fonts/joystix monospace.otf");
		m_textRenderer.transform.position = glm::vec3(25.0f, AppCam->Height - 36, 0.0f);
		m_textRenderer.transform.scale = 0.5f;
		m_textRenderer.cachedDraw = true;

		hiScorePosition = glm::vec3(25.0f, AppCam->Height - (36 * 2), 0.5f);
		elpasedTimePosition = glm::vec3(25.0f, AppCam->Height - (36 * 3), 0.5f);


		CanvasItem::canvasText = &m_textRenderer; //set the canva's textrender reference 1st!
		//canvas
		{ 

			back_button = new CanvasText("Return To Main Menu", glm::vec2(25.0f, 514), glm::vec4(0, 1.0, 1.0, 1.0f), true, true);
			back_button->func = new Callback(SpaceBusterIIISetup::Wrap_NavToScene, this, "Final", "MenuScene");
			back_button->transform.scale = 1.0f;


			restart_button = new CanvasText("Restart", glm::vec2(25.0f, 414), glm::vec4(0, 1.0, 1.0, 1.0f), false, true);
			restart_button->func = new Callback(SpaceBusterIIISetup::Wrap_NavToScene, this, "Final", "GameSetup");

			uiCanvas.children.emplace_back(back_button);
			uiCanvas.children.emplace_back(restart_button);
		}

		canvasProjectionMatrix = glm::ortho(0.f, (float)AppCam->Width, 0.f, (float)AppCam->Height);
		CanvasShader->Use().SetMatrix4("projection", canvasProjectionMatrix);
		TextRenderer::fontShader->Use().SetMatrix4("projection", canvasProjectionMatrix);

		//Load score data
		m_scores = new FileIO::ScoreFile("./assets/Scores.json");
		char hiscorconstchar[17];
		sprintf_s(hiscorconstchar, "%010d", m_scores->highestScore);
		hiScoreText = "Hi Score: "; hiScoreText += hiscorconstchar;



		//config screen contrainer aka mission
		MissionDefinition missionDef;
		missionDef.contrainObjectsToScreen = true;
		missionDef.hasTimeLimit = false;
		contrainAstroidsToscreen = new MissionSurvival(&missionDef);


		ObjectFactory::SetObjectListener(contrainAstroidsToscreen);
	};
	//destructor
	~SpaceBusterIIISetup()
	{
		FileIO::SaveFile("./assets/Scores.json", m_scores);
		delete m_scores;
		CanvasItem::canvasText = nullptr;
		delete astroidFactory;
		delete shipController;
		delete CanvasShader;


		ObjectFactory::SetObjectListener(nullptr);
		ObjectFactory::cleanup();
		delete contrainAstroidsToscreen;

		back_button = nullptr;
		restart_button = nullptr;
	};

	static void Wrap_NavToScene(SpaceBusterIIISetup* instance, const char* category, const char* title) { instance->NavToScene(category, title); }
	void NavToScene(const char* categoryName, const char* sceneName)
	{
		printf("Navigating to  Scene: '%s/%s'!\n", categoryName, sceneName);
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

	void KeyboardUp(int key) override { shipController->onKeyRelease(key); }
	void KeyboardDown(int key)override { shipController->onKeyPress(key); }
	virtual void processInput(GLFWwindow* window, float deltaTime) override { }

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
		shipController->mouseCallback(button, action, mode);

		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			glm::vec2 mousePos = glm::vec2((float)x, (float)y);
			mousePos = ScreenToWorldCoords_projectionMatOnly(canvasProjectionMatrix, mousePos);

			for (int i = 0; i < uiCanvas.children.size(); i++)
			{
				//a bit ineffecient, but at least it doesn't run every frame
				if (uiCanvas.children[i]->active && uiCanvas.children[i]->isPointInBounds(mousePos, &uiCanvas.t))
				{
					uiCanvas.children[i]->handleCallback();
					return;
				}
				else if (App::g_App->state == App::App_State::state_paused)
				{//then...
					if (uiCanvas.children[i]->isPointInBounds(mousePos, &uiCanvas.t))
					{
						uiCanvas.children[i]->handleCallback();
					}
				}
			}
		}
	}
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) override 
	{
		shipController->mouseCallback_Cursor(x, y);

		glm::vec2 mousePos = glm::vec2((float)x, (float)y);

		CanvasItem::mousePosition = ScreenToWorldCoords_projectionMatOnly(canvasProjectionMatrix, mousePos);

		if (App::g_App->state == App::App_State::state_paused)
		{
			for (int i = 0; i < uiCanvas.children.size(); i++)
			{
				if (uiCanvas.children[i]->isPointInBounds(CanvasItem::mousePosition, &uiCanvas.t))
				{
					uiCanvas.children[i]->active = true;
				}
				else
				{
					uiCanvas.children[i]->active = false;
				}
			}
		}

	}
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override {}
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override 
	{
	

		canvasProjectionMatrix = glm::ortho(0.f, (float)AppCam->Width, 0.f, (float)AppCam->Height);
		CanvasShader->Use().SetMatrix4("projection", canvasProjectionMatrix);
		TextRenderer::fontShader->Use().SetMatrix4("projection", canvasProjectionMatrix);

	}

	//frame updates
	void FixedUpdate(float dt) override 
	{ 
		shipController->FixedUpdate(dt);
		ObjectFactory::FixedUpdateObjects(dt);
	}
	void Update(float dt) override {
		if (shipController->GetShip()->GetAttributes()->getCondition() <= 0.0f && shipController->condactIterationDone())
		{
			App::g_App->state = App::state_paused;
			restart_button->visible = true;
		}


		shipController->Update(dt);
		astroidFactory->Update(dt);

		ObjectFactory::UpdateObjects(dt);

		contrainAstroidsToscreen->Update(dt);
		uiCanvas.Update(dt);

		m_scores->currentScore = contrainAstroidsToscreen->scoreManager->getScore();
	}

	//collision callbacks 
	void onBeginContact(b2Contact* contact) override { shipController->handleBeginContact(contact); ObjectFactory::handleBeginContact(contact); }
	void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override { shipController->handlePreSolve(contact, oldManifold); ObjectFactory::handlePreSolve(contact, oldManifold); }
	void onEndContact(b2Contact* contact) override { shipController->handleEndContact(contact); ObjectFactory::handleEndContact(contact); }
	void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override { shipController->handlePostSolve(contact, impulse); ObjectFactory::handlePostSolve(contact, impulse); }

	//screen drawing

	void DrawScene() override 
	{

		glm::mat4 model = glm::mat4(1);
		glm::mat4 projection = AppCam->getProjectionMatrix();
		glm::mat4 view = AppCam->GetViewMatrix();

		m_Shader->Use()
			.SetMatrix4("projection", projection)
			.SetMatrix4("view", view);


		DrawBackGround();

		DrawForeGround();

		m_World->DrawDebugData();
		m_physDrawer->SetFlags_Internal();
		m_physDrawer->setMat4(projection * view);
		m_physDrawer->Flush();
	} 

	void DrawBackGround() override 
	{
		//draw parallax background
		

		//draw game objects
		ObjectFactory::DrawObjects(m_Shader, m_surface);
		shipController->Draw(m_Shader, m_surface);
	}
	void DrawForeGround() override 
	{
		uiCanvas.children[0]->visible = App::g_App->state == App::App_State::state_paused;
		//draw ui
		uiCanvas.Draw(CanvasShader, m_surface);
		std::string currentScore = "Score:    ";
		//std::string currentScore = "Hi Score: ";


		char buffer[17]; sprintf_s(buffer, "%010d", m_scores->currentScore);
		currentScore += buffer;

		//draw current score
		m_textRenderer.DrawText(currentScore.c_str());
		//draw last recorded hi score
		m_textRenderer.DrawText(hiScoreText.c_str(), hiScorePosition.x, hiScorePosition.y, hiScorePosition.z);
		//draw elapsed time
		std::string elapsedTimeStr = this->contrainAstroidsToscreen->getTimeLeft_string();
		m_textRenderer.DrawText(elapsedTimeStr.c_str(), elpasedTimePosition.x, elpasedTimePosition.y, elpasedTimePosition.z);

	}

	void DrawDebug() override 
	{
		//text positions
		ImGui::SliderFloat3("hi score pos", &hiScorePosition.x, -AppCam->Height, AppCam->Height);
		ImGui::SliderFloat3("elpsd time pos", &elpasedTimePosition.x, -AppCam->Height, AppCam->Height);

		ImGui::Text("Constrainer aka 'mission'");
		ImGui::Separator();
		contrainAstroidsToscreen->Debug();

		ImGui::Text("UI Members");
		ImGui::Separator();
		m_textRenderer.Debug();
		uiCanvas.Debug();

		ImGui::Text("Game Objects");
		ImGui::Separator();
		shipController->Debug();
		astroidFactory->Debug();
		ObjectFactory::Debug();

		ImGui::Text("Physics Debug");
		ImGui::Separator();
		m_physDrawer->renderDebug();
	}

	//creation
	static Game* Create() { return new SpaceBusterIIISetup; }
};

static int testIndex = RegisterGame("Final", "GameSetup", SpaceBusterIIISetup::Create);
