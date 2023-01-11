#include "Game.h"

#include "SpriteMap.h"
#include "Sprite.h"

#include "imgui.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "ScrollingSprite.h"
struct spriteWrapper {
	spriteWrapper(Texture* texture, const int columns, const int rows)
		: sprite(new Sprite(texture, columns, rows))
		, position(glm::vec3(0.0f)), size(glm::vec3(1.0f))
		, pivot(glm::vec3(0.0f))
		, scale(1.0f)
		, flipHorizontal(false)
		, flipVertical(false)
		, drawReflection(false)
		, spritedir(1.0f)
		, reflectionColor(0.0f, 0.0f, 0.0f, 1.0f)
	{
	}
	~spriteWrapper()
	{
		delete sprite;
	}
	Sprite* sprite;
	glm::vec3 position, pivot, size;
	glm::float32 scale;
	bool flipHorizontal, flipVertical, drawReflection;
	glm::vec3 spritedir;
	glm::vec4 reflectionColor;
	void Update(float dt) { 
		sprite->Update(dt); 
	}
	void Draw(Shader* shader) {

		glm::mat4 model(1.0f);

		model = glm::translate(model, pivot);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(size.x * spritedir.x * scale, size.y * spritedir.y * scale, size.z * scale) );

		model = glm::translate(model, -pivot);

		shader->SetMatrix4("model", model);
		sprite->Draw(shader);

		if (drawReflection)
		{
			glm::mat4 model(1.0f);

			model = glm::translate(model, pivot);
			model = glm::translate(model, position);
			model = glm::scale(model, glm::vec3(size.x * spritedir.x * scale, - size.y * spritedir.y * scale, size.z * scale));

			model = glm::translate(model, -pivot);

			shader->SetMatrix4("model", model);
			sprite->Draw(shader, &reflectionColor);

		}

	}
	void Debug(const char* name) {
		if (ImGui::TreeNode(name))
		{
			if (ImGui::Checkbox("Flip Horiz", &flipHorizontal))
			{
				spritedir.x = flipHorizontal ? -1.0f : 1.0f;
			}
			if (ImGui::Checkbox("Flip Vert", &flipVertical))
			{
				spritedir.y = flipVertical ? -1.0f : 1.0f;
			}
			ImGui::Checkbox("reflect", &drawReflection);
			if (ImGui::TreeNode("-transform-"))
			{
				ImGui::SliderFloat3("pos", &position.x, -10, 10);
				ImGui::SliderFloat3("pivot", &pivot.x, -0.5, 0.5);
				ImGui::SliderFloat3("size", &size.x, 0.1, 10);
				ImGui::SliderFloat("scale", &scale, 0.1, 10);
		
				ImGui::TreePop();
			}
			sprite->Debug();

			ImGui::TreePop();
		}
	}

};

class ParallaxTest : public Game
{
public:
	//constructor
	spriteWrapper* spriteObj;
	Texture *foregroundTexture, *BackgroundTexture01, *BackgroundTexture02, *BackgroundTexture03;
	GameObject* brickobject;

	ScrollingSprite* background;

	ParallaxTest() : Game()
	{
		//load textures
		foregroundTexture = new Texture("./assets/Haohmaru_141.png", true);
		BackgroundTexture01 = new Texture("./assets/bgs/foreground_00.png", true);
		BackgroundTexture02 = new Texture("./assets/bgs/foreground_01.png", true);
		BackgroundTexture03 = new Texture("./assets/bgs/foreground_02.png", true);

		//set up background
		ScrollingSpriteDefinition ssd;
		ssd.layerCount = 3;
		ssd.layers.emplace_back(BackgroundTexture03);
		ssd.layers.emplace_back(BackgroundTexture02);
		ssd.layers.emplace_back(BackgroundTexture01);
		ssd.position = glm::vec3(-10.0f, 0.0f, 0.0f);
		ssd.pivot = glm::vec3(-0.5f, -0.5, 0.0f);
		ssd.size = glm::vec3(3.784f, 1.0f, 1.0f);
		ssd.scale = 7.459f;

		background = new ScrollingSprite(&ssd);



		{
			//intialize foreground elements
			spriteObj = new spriteWrapper(foregroundTexture, 23, 23);
			//configure the size to be comparable w/ a normal game object of size 1,1,1
			spriteObj->size = glm::vec3(3.0f, 3.0f, 1.0f);
			//spriteObj->pivot = glm::vec3(0.0, -0.385f, 0.0f);
			spriteObj->pivot = glm::vec3(0.0, -0.407f, 0.0f);
			//set up initial animation frame
			spriteObj->sprite->animStartIndex = 87;
			spriteObj->sprite->animEndIndex = 92;
			spriteObj->sprite->map->GetFrame(0);
			spriteObj->sprite->animate = true;

		}


		//change shader
		m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);
		//reconfig cam
		AppCam->zoom = 0.112f;

		{ //for scaling
			GameObjectDefinition def;
			def.position = glm::vec3(0.0f);
			def.texture = ResourceManager::GetTexturePtr("icon");
			def.enabled_physics = false;
			def.visible = false;
			def.name = "Brick object";

			brickobject = new GameObject(&def);
		}
	};
	//destructor
	~ParallaxTest()
	{
		delete foregroundTexture;
		delete BackgroundTexture01;
		delete BackgroundTexture02;
		delete BackgroundTexture03;

		delete spriteObj;
		delete brickobject;

		delete background;
	};
	//mouse callbacks
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override {}
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override {}

	void Update(float dt) override {
		spriteObj->Update(dt);
		background->Update(dt);
	}


	//screen drawing
	void DrawScene() override 
	{

		glm::mat4 model = glm::mat4(1);
		glm::mat4 projection = AppCam->getProjectionMatrix();
		glm::mat4 view = AppCam->GetViewMatrix();


		m_Shader->Use()
			.SetMatrix4("projection", projection)
			.SetMatrix4("view", view)
			.setInt("image", 0);

		DrawBackGround();
		DrawForeGround();
	} 
	void DrawBackGround() override 
	{
		background->Draw(m_Shader, m_surface);

	}
	void DrawForeGround() override 
	{
		brickobject->Draw(m_Shader, m_surface);
		spriteObj->Draw(m_Shader);
	}
	void DrawDebug() override 
	{
		background->Debug();
		spriteObj->Debug("Haomaru-sprite wrapper");
		brickobject->Debug();
	}

	//creation
	static Game* Create() { return new ParallaxTest; }
};

static int testIndex = RegisterGame("Scene", "Parallax Test", ParallaxTest::Create);  //do not actually register this lol
