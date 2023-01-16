#include <glad/glad.h>

#include "Game.h"

#include "imgui.h"
#include "Utility_Physics.h"
#include "Sprite.h"


class SpriteMapTest : public Game
{
public:
	
	Texture* t_texture, *k_texture;
	glm::vec3 position = glm::vec3(0, -0.160f, 0.01f);

	Sprite* m_sprite,* k_sprite;

	//constructor
	SpriteMapTest() : Game()
	{
		//m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);
		m_Shader = new Shader("textureAtlas.vts", "textureAtlas.frs", true);

		t_texture = new Texture("./assets/Haohmaru_141.png", true);
		k_texture = new Texture("./assets/testyourAtlas.png", true);

		{
			m_sprite = new Sprite(t_texture, 23, 23);
			m_sprite->frameStart = 87;
			m_sprite->frameEnd = 92;
			m_sprite->index = 0;
			m_sprite->animated = true;
		}


		k_sprite = new Sprite(k_texture, 7, 7);
		k_sprite->transform.position.z = 0.5f;

		k_sprite->transform.UpdateMatrix();
		AppCam->zoom = 0.045f;
		AppCam->position = glm::vec3(0.609f, -0.123f, 0.0f);

		//printf("Enabling depth test\n");
		glEnable(GL_DEPTH_TEST);

	};
	//destructor
	~SpriteMapTest()
	{
		delete t_texture;
		delete k_texture;
		
		delete m_sprite;
		delete k_sprite;

		//printf("disabling depth test\n");
		glDisable(GL_DEPTH_TEST);
	};

	//frame updates
	//void FixedUpdate(float dt) override { }
	void Update(float dt) override 
	{
		m_sprite->Update(dt);
		k_sprite->Update(dt);
	}
	void DrawScene() override {

		glm::mat4 projection = AppCam->getProjectionMatrix();
		glm::mat4 view = AppCam->GetViewMatrix();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);



		m_Shader->Use()
			.SetMatrix4("projection", projection)
			.SetMatrix4("view", view)
			.setInt("image", 0);

		t_texture->Bind();

		m_sprite->Draw(m_Shader, m_surface);
		k_sprite->Draw(m_Shader, m_surface);
		

		DrawBackGround();



		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		DrawBackGround();
		DrawForeGround();


	}
	void DrawDebug() override {

		if (ImGui::Button("Hot reload k shader"))
		{
			delete m_Shader;
			m_Shader = new Shader("textureAtlas.vts", "textureAtlas.frs", true);
		}


		ImGui::SliderFloat3("pos 01", &position.x, -10, 10);
		m_sprite->Debug("haomaru sprite");
		k_sprite->Debug("test sprite");

	}

	//creation
	static Game* Create() { return new SpriteMapTest; }
};

static int testIndex = RegisterGame("Scene", "Test Sprite Map", SpriteMapTest::Create);  //do not actually register this lol
