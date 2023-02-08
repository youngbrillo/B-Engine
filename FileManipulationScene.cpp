#include "Game.h"
#include "imgui.h"
#include "TextRenderer.h"
 
// read from a high score file
// get current high score
// save current score to high score file

#include "FileIO.h"
#include "FileData.h"
#include "TextRenderer.h"

class FileManipulationScene : public Game
{
public:
	//constructor

	FileIO::ScoreFile* m_scores = nullptr;
	std::string scoreString, scoreString_highest;
	TextRenderer trend;
	bool canSave = true;
	FileManipulationScene() : Game()
	{
		m_scores = new FileIO::ScoreFile("./assets/Scores.json");

		trend = TextRenderer("./assets/fonts/joystix monospace.otf");
		trend.transform.position = glm::vec3(25.0f, AppCam->Height - 72.0f, 0.0f);
		trend.transform.scale = 0.5f;

		scoreString = "Score: ";
		scoreString_highest = "Hi Score: " + std::to_string(m_scores->highestScore);

	};
	//destructor
	~FileManipulationScene()
	{
		if(canSave)
			FileIO::SaveFile("./assets/Scores.json", m_scores);

		delete m_scores;
	};

	//mouse callbacks
	virtual void ResizeWindowCallback(GLFWwindow* window, int width, int height) override {
		Game::ResizeWindowCallback(window, width, height);
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Game::AppCam->Width), 0.0f, static_cast<float>(Game::AppCam->Height));
		TextRenderer::fontShader->Use().SetMatrix4("projection", projection);
	}
	//key callbacks and frame input key callback
	void KeyboardDown(int key)override 
	{ 
		if (key == GLFW_KEY_UP) 
		{
			m_scores->currentScore += 1;
			//printf("Score: %d\n", m_scores->currentScore);
		}
		if (key == GLFW_KEY_RIGHT) 
		{
			m_scores->currentScore += 10;
			//printf("Score: %d\n", m_scores->currentScore);
		}


		if (m_scores->currentScore > m_scores->highestScore)
		{
			m_scores->highestScore = m_scores->currentScore;
		}
	
	}
	void Update(float dt) override 
	{ 
		scoreString = "Score: ";
		scoreString += std::to_string(m_scores->currentScore);
		scoreString_highest = "Hi Score: ";
		scoreString_highest += std::to_string(m_scores->highestScore);
	}
	//screen drawing
	void DrawForeGround() override 
	{ 
		trend.DrawText(scoreString.c_str());
		trend.DrawText(scoreString_highest.c_str(), trend.transform.position.x, trend.transform.position.y - 72.0f / 2 * trend.transform.scale, trend.transform.scale, trend.fontColor);

	}
	//void DrawScene() override {} 
	void DrawDebug() override 
	{
		ImGui::Checkbox("enable file saving", &canSave);
		trend.Debug();
	}

	//creation
	static Game* Create() { return new FileManipulationScene; }
};

static int testIndex = RegisterGame("Test", "File Saving/Loading", FileManipulationScene::Create);  
