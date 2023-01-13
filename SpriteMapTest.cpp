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
	Texture* t;
	glm::vec3 position = glm::vec3(0, -0.160f, 0.01f);
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


		AppCam->zoom = 0.045f;
		AppCam->position = glm::vec3(0.609f, -0.123f, 0.0f);

		printf("Enabling depth test\n");
		glEnable(GL_DEPTH_TEST);

	};
	//destructor
	~SpriteMapTest()
	{
		delete t;
		//delete k;

		delete m_sprite;
		//delete k_sprite;

		printf("disabling depth test\n");
		glDisable(GL_DEPTH_TEST);
	};

	//frame updates
	//void FixedUpdate(float dt) override { }
	void Update(float dt) override 
	{
		m_sprite->Update(dt);
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

		DrawBackGround();



		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		DrawBackGround();
		DrawForeGround();


	}
	void DrawDebug() override {

		ImGui::SliderFloat3("pos 01", &position.x, -10, 10);
		m_sprite->Debug();

	}

	//creation
	static Game* Create() { return new SpriteMapTest; }
};

static int testIndex = RegisterGame("Scene", "Test Sprite Map", SpriteMapTest::Create);  //do not actually register this lol
