#include <glad/glad.h>

#include "Game.h"

#include "imgui.h"
#include "Utility_Physics.h"
#include "TextureAtlas.h"


class SpriteMapTest : public Game
{
public:

	glm::vec4 textColor = glm::vec4(1.0f);
	glm::vec4 spriteColor = glm::vec4(1);//glm::vec4(1, 0.5, 1, 1);

	glm::vec2 textureMinMax = glm::vec2(0.0f, 1.0f);

	glm::vec3 spriteSize = glm::vec3(1.0f * 2.38432835821f, 1.0f, 1.0f);
	glm::vec3 spriteOrientationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 spriteRotateAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 spritePosition = glm::vec3(0);

	float spriteScale = 1.0f, elapsed = 1.0f;
	bool isQuad = true, autoRotate = false;
	float angle = 0.0f, angle2 = 0.0f;


	TextureAtlas m_sprite;
	//constructor
	SpriteMapTest() : Game()
	{
		//m_Shader = new Shader("test.vts", "test.frs", true);
		m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);
		m_sprite = TextureAtlas("./assets/spriteAtlas.png", 42, 11);

		//m_Shader = new Shader("test.vts", "test.frs", true);
		//m_surface = new quadSurface(); m_surface->Generate();
		AppCam->zoom = 0.053;
		AppCam->position = glm::vec3(0.352f, 0.130f, 0.0f);

		printf("Enabling depth test\n");
		glEnable(GL_DEPTH_TEST);

	};
	//destructor
	~SpriteMapTest()
	{
		delete m_surface;
		m_surface = nullptr;
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
		if (autoRotate)
			elapsed += dt;
	}

	//collision callbacks 
	void onBeginContact(b2Contact* contact) override {}
	void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override {}
	void onEndContact(b2Contact* contact) override {}
	void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override {}

	//screen drawing
	void DrawBackGround() override {}
	void DrawForeGround() override {
		//DrawString(5, 0, "Implement Sprite functionality", textColor.x, textColor.y, textColor.z, textColor.w);
		//DrawString(15, 15, "1. Creation of texture and surface classes", textColor.x, textColor.y, textColor.z, textColor.w);
		//DrawString(15, 30, "2. Join them in one interface, 'Sprite'", textColor.x, textColor.y, textColor.z, textColor.w);
		////DrawString(15, 45, "3. ");
	}
	void DrawScene() override {
		//
		//printf("game, shader value %d\n", m_Shader->GetId());

		glm::mat4 model = glm::mat4(1);
		glm::mat4 projection = AppCam->getProjectionMatrix();
		glm::mat4 view = AppCam->GetViewMatrix();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);


		model = glm::translate(model, spritePosition);
		model = glm::rotate(model, angle, spriteOrientationAxis);
		model = glm::rotate(model, (angle2 * elapsed), spriteRotateAxis);
		model = glm::scale(model, spriteSize * spriteScale);

		m_Shader->Use()
			.SetMatrix4("projection", projection)
			.SetMatrix4("view", view);

		DrawBackGround();

		//glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

		m_Shader->Use()
			.SetMatrix4("model", model)
			.SetVector4f("color", spriteColor)
			.setInt("image", 0);


		{

			m_sprite.Bind();
			m_surface->Bind();
		}


		{
			//m_sprite.DrawSprite()
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		DrawBackGround();
		DrawForeGround();


	}
	void DrawDebug() override {
		ImGui::ColorEdit4("sprite Color", &spriteColor.x);
		ImGui::ColorEdit4("Text Color", &textColor.x);

		if (ImGui::SliderFloat2("Texture min max", &textureMinMax.x, 0, 5.0f))
		{
			delete m_surface;
			if (isQuad)
				m_surface = new quadSurface();
			else
				m_surface = new cubeSurface();
			m_surface->Generate(textureMinMax.x, textureMinMax.y);
		}
		m_sprite.RenderDebug();
		/*
			glm::vec3 spriteSize = glm::vec3(1.0f, 1.0f, 1.0f);
			glm::vec3 spriteRotateAxis = glm::vec3(0.0f, 0.0f, 1.0f);
			glm::vec3 spritePosition = glm::vec3(0);

			float spriteScale = 1.0f, elapsed = 1.0f;
			bool isQuad = true, autoRotate = false;
			float angle;
		*/
		ImGui::SliderFloat3("sprite pos", &spritePosition.x, -5, 5);
		ImGui::SliderFloat3("sprite size", &spriteSize.x, 0, 10);
		ImGui::SliderFloat("sprite scale", &spriteScale, 0, 15);

		ImGui::SliderAngle("sprite Angle", &angle);
		ImGui::SliderFloat3("sprite axis", &spriteOrientationAxis.x, -1, 1);


		ImGui::SliderAngle("rotation Angle", &angle2);
		ImGui::SliderFloat3("rotation axis 2", &spriteRotateAxis.x, -1, 1);


		if (ImGui::Checkbox("Quad surface?", &isQuad))
		{

			delete m_surface;
			if (isQuad)
				m_surface = new quadSurface();
			else
				m_surface = new cubeSurface();

			m_surface->Generate(textureMinMax.x, textureMinMax.y);
		}
		if (ImGui::Checkbox("Auto Rotate?", &autoRotate))
		{
			elapsed = autoRotate ? 0.0f : 1.0f;
		}
	}

	//creation
	static Game* Create() { return new SpriteMapTest; }
};

static int testIndex = RegisterGame("Scene", "Test Sprite Map", SpriteMapTest::Create);  //do not actually register this lol
