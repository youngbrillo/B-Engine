#include <glad/glad.h>

#include "Game.h"

#include "imgui.h"
#include "Utility_Physics.h"
#include "SpriteMap.h"
#include "Sprite.h"

class SpriteMapTest : public Game
{
public:
	
	Sprite* m_sprite, *k_sprite;
	Texture* t, *k;
	glm::vec3 position = glm::vec3(0, -0.160f, 0.01f);
	glm::vec3 position2 = glm::vec3(1, -0.160f, 0.01f);
	//constructor
	SpriteMapTest() : Game()
	{
		m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);
		{
			t = new Texture("./assets/Haohmaru_141.png", true);

			m_sprite = new Sprite(t, 23, 23);
			m_sprite->animStartIndex = 87;
			m_sprite->animEndIndex = 92;
			m_sprite->map->GetFrame(0);
			m_sprite->animate = true;
		}

		{
			k = new Texture("./assets/his13_000-sheet.png", true);
			k_sprite = new Sprite(k, 7, 6);
			k_sprite->animStartIndex = 18;
			k_sprite->animEndIndex = 27;
			k_sprite->map->GetFrame(0);
			k_sprite->animate = true;
		}

		AppCam->zoom = 0.045f;
		AppCam->position = glm::vec3(0.609f, -0.123f, 0.0f);

		printf("Enabling depth test\n");
		glEnable(GL_DEPTH_TEST);

	};
	//destructor
	~SpriteMapTest()
	{
		delete t;
		delete k;

		delete m_sprite;
		delete k_sprite;

		printf("disabling depth test\n");
		glDisable(GL_DEPTH_TEST);
	};

	//key callbacks and frame input key callback
	void KeyboardUp(int key) override { }
	void KeyboardDown(int key)override { }
	virtual void processInput(GLFWwindow* window, float deltaTime) override { }
	//mouse callbacks
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override {}
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) override {}
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override {}
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override {}

	//frame updates
	void FixedUpdate(float dt) override { }
	void Update(float dt) override {
		m_sprite->Update(dt);
		k_sprite->Update(dt);
	}

	//collision callbacks 
	void onBeginContact(b2Contact* contact) override {}
	void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override {}
	void onEndContact(b2Contact* contact) override {}
	void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override {}

	//screen drawing
	void DrawBackGround() override {}
	void DrawForeGround() override {

	}
	void DrawScene() override {

		glm::mat4 model = glm::mat4(1);
		glm::mat4 projection = AppCam->getProjectionMatrix();
		glm::mat4 view = AppCam->GetViewMatrix();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);



		m_Shader->Use()
			.SetMatrix4("projection", projection)
			.SetMatrix4("view", view)
			.SetMatrix4("model", model)
			.SetVector4f("color", glm::vec4(1.0f))
			.setInt("image", 0);

		t->Bind();
		m_surface->Bind();
		{
			model = glm::translate(model, position);
			m_Shader->SetMatrix4("model", model);
			m_sprite->Draw(m_Shader);

		}
		{
			model = glm::translate(model, position2);
			m_Shader->SetMatrix4("model", model);
			k_sprite->Draw(m_Shader);

		}
		DrawBackGround();



		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		DrawBackGround();
		DrawForeGround();


	}
	void DrawDebug() override {

		ImGui::SliderFloat3("pos 01", &position.x, -10, 10);
		ImGui::SliderFloat3("pos 02", &position2.x, -10, 10);
		m_sprite->Debug();

	}

	//creation
	static Game* Create() { return new SpriteMapTest; }
};

static int testIndex = RegisterGame("Scene", "Test Sprite Map", SpriteMapTest::Create);  //do not actually register this lol
