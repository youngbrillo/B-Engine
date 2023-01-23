
#include<glad/glad.h>

#include "Game.h"

#include "imgui.h"
/*
	use this scene to test out more efficient ways to render to the screen!
	expiriment w/ points, lines, line-strips, line_loops, triangles, triangle_strips, and triangle_fans
	until you have mastered the concepts!!


	//to think that the solution to texture atlasing was in the shaders the whole time....
*/

#include "Texture.h"
#include "Transform.h"

#include "Sprite.h"
#include "SpriteAnimator.h"

class RenderTestScene: public Game
{
public:

	bool cullFaces = false;

	Sprite* spriteObj;
	Texture* texture;
	SpriteAnimator* animationController;

	RenderTestScene() : Game()
	{
		//initialize the render pipeline....
		texture = new Texture("./assets/Haohmaru_141.png", true);
		m_Shader = new Shader("textureAtlas.vts", "textureAtlas.frs", true);

		m_Shader->Use().setInt("image", 0);
		AppCam->zoom = 0.039f;
		AppCam->position = glm::vec3(0.313f, -0.216f, 0.0f);

		spriteObj = new Sprite(texture, 23, 23);
		animationController = new SpriteAnimator(spriteObj, "./assets/Haohmaru_141.json");

		animationController->setAnimation(0);
	};
	//destructor
	~RenderTestScene()
	{
		delete animationController;
		delete spriteObj;
		delete texture;
	};

	//key callbacks and frame input key callback
	void KeyboardUp(int key) override { }

	void Keyboard(int key) override
	{

		if (key == GLFW_KEY_RIGHT) {
			//spriteObj.index = (index + 1) % ((int)(rowColumn.x * rowColumn.y) + 1);
		}
		if (key == GLFW_KEY_LEFT)
		{
			//spriteObj.index = (index - 1) % ((int)(rowColumn.x * rowColumn.y) + 1);
			//if (index <= 0) index = ((int)(rowColumn.x * rowColumn.y));
		}
	}
	void KeyboardDown(int key)override 
	{ 
		if (key == GLFW_KEY_RIGHT) {
			//spriteObj.index = (spriteObj.index + 1) % ((int) (rowColumn.x * rowColumn.y) + 1);
		}
		if (key == GLFW_KEY_LEFT)
		{
			//spriteObj.index = (ispriteObj.ndex -1) % ((int)(rowColumn.x * rowColumn.y) + 1);
			//if (index <= 0) index = ((int)(rowColumn.x * rowColumn.y));
		}
	}

	void Update(float dt) override 
	{ 

		spriteObj->Update(dt);
		animationController->Update(dt);
	}

	//screen drawing
	void DrawScene() override 
	{
		if (cullFaces)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CW);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		m_Shader->Use()
			.SetMatrix4("projection", AppCam->getProjectionMatrix())
			.SetMatrix4("view", AppCam->GetViewMatrix());
		texture->Bind();

		spriteObj->Draw(m_Shader, m_surface);

		//m_surface->Bind();



	} 
	void DrawDebug() override 
	{

		animationController->Debug();
		spriteObj->Debug("HaoMaru sprite");

		ImGui::Checkbox("Face culling", &cullFaces);
		if (ImGui::Button("Hot reload Shader"))
		{
			printf("Hot reloading shader\n");
			delete m_Shader;
			m_Shader = new Shader("textureAtlas.vts", "textureAtlas.frs", true);
		}
	}

	//creation
	static Game* Create() { return new RenderTestScene; }
};

static int testIndex = RegisterGame("Test", "Animation/State", RenderTestScene::Create);  
