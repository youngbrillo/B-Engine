#include "Game.h"

#include "Sprite.h"

#include "imgui.h"
#include "GameObject.h"

#include "ResourceManager.h"

#include "ParallaxLayer.h"
#include "SpriteAnimator.h"




class ParallaxTest : public Game
{
public:
	//constructor
	Sprite* spriteObj, *testSprite;
	Texture* foregroundTexture, * bg01, * bg02, * bg03, * bg04, * bg05, * bg06;
	GameObject* brickobject;

	ParallaxManager* background;


	glm::ivec2 sceneDirection;
	SpriteAnimator* animationController;
	bool shiftkeyhelddown = false, bindDirection = true;

	ParallaxTest() : Game()
	{
		//load textures
		foregroundTexture = new Texture("./assets/Haohmaru_141.png", true);
		bg01 = new Texture("./assets/bgs/foreground_00.png", true);
		bg02 = new Texture("./assets/bgs/foreground_01.png", true);
		bg03 = new Texture("./assets/bgs/foreground_02.png", true);
		bg04 = new Texture("./assets/bgs/foreground_04.png", true);
		bg05 = new Texture("./assets/bgs/background_settingSun.png", true);
		bg06 = new Texture("./assets/bgs/foreground_05.png", true);

		//set up background
		ParallaxManagerDefinition ssd;
		//ssd.layerCount = 6;
		ssd.layers.emplace_back(new ParallaxLayer(bg06, 6, 1.736 / 6, "sky backdrop", false));
		ssd.layers.emplace_back(new ParallaxLayer(bg05, 5, 1.736 / 5, "Sun", false));
		ssd.layers.emplace_back(new ParallaxLayer(bg04, 4, 1.736 / 4, "clouds"));
		ssd.layers.emplace_back(new ParallaxLayer(bg03, 3, 1.736 / 3, "city 02"));
		ssd.layers.emplace_back(new ParallaxLayer(bg02, 2, 1.736 / 2, "city 01"));
		ssd.layers.emplace_back(new ParallaxLayer(bg01, 1, 1.736 / 1, "road"));

		ssd._transform.position = glm::vec3(-10.0f, 0.0f, 0.0f);
		ssd._transform.pivot = glm::vec3(-0.5f, -0.5, 0.0f);
		ssd._transform.size = glm::vec3(3.784f, 1.0f, 1.0f);
		ssd._transform.scale = 7.459f;

		background = new ParallaxManager(&ssd);
		background->animated = true;
		background->visible = true;
		background->animationEnd = 16.00f; 

		//set scene direction to 
		sceneDirection = glm::ivec2(0, 0);

		{
			//intialize foreground elements
			spriteObj = new Sprite(foregroundTexture, 23, 23);
			//configure the size to be comparable w/ a normal game object of size 1,1,1
			spriteObj->transform.size = glm::vec3(3.0f, 3.0f, 1.0f);
			spriteObj->transform.pivot = glm::vec3(0.0, -0.407f, 0.0f);
			spriteObj->transform.UpdateMatrix();
			//set up initial animation frame
			spriteObj->frameStart = 0;
			spriteObj->frameEnd = 9;
			spriteObj->index = 0;
			spriteObj->animated = true;


		}

		animationController = new SpriteAnimator(spriteObj, "./assets/Haohmaru_141.json");
		//animationController->addAnimation(new Animation("m_idle", 0, 9), true);
		//animationController->addAnimation(new Animation("m_walk", 26, 33));
		//animationController->addAnimation(new Animation("m_walkBack", 34, 41));
		//animationController->addAnimation(new Animation("m-dash", 80, 86, true));
		//animationController->addAnimation(new Animation("m_run", 87, 92));
		//animationController->addAnimation(new Animation("m_dash-back", 93, 97, true));
		//animationController->addAnimation(new Animation("m_grab-mid", 96, 101, true));
		//animationController->addAnimation(new Animation("m_kick", 291, 300, true));

		{
			testSprite = new Sprite(bg01, 1, 1);

			testSprite->transform.position = glm::vec3(-10.0f, 0.0f, 0.0f);
			testSprite->transform.pivot = glm::vec3(-0.5f, -0.5, 0.0f);
			testSprite->transform.size = glm::vec3(3.784f, 1.0f, 1.0f);
			testSprite->transform.scale = 7.459f;
			
			testSprite->transform.UpdateMatrix();
			testSprite->visible = false;
		}
		//assign animations
		//animationController->setAnimation(0);

		//change shader
		//m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);
		m_Shader = new Shader("textureAtlas.vts", "textureAtlas.frs", true);
		//reconfig cam

		AppCam->zoom = 0.123f;
		AppCam->position = glm::vec3(0.43f, 1.563f, 0.0f);
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
		delete spriteObj;
		delete testSprite;
		delete brickobject;
		delete background;

		delete foregroundTexture;
		delete bg01;
		delete bg02;
		delete bg03;
		//delete bg04;
		//delete bg05;
		//delete bg06;


	};
	//mouse callbacks
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override {}
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override {}

	void Update(float dt) override {
		spriteObj->Update(dt);
		testSprite->Update(dt);
		if (bindDirection)
		{
			background->direction = this->sceneDirection;
			background->baseSpeed = this->shiftkeyhelddown ? 2.0f : 0.25f;
		}
		background->Update(dt);
		animationController->Update(dt);
	}

	void KeyboardDown(int key) override
	{
		bool valid = false;
		glm::ivec2 lastDirection = sceneDirection;
		if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
		{
			sceneDirection.x++; 
			if (sceneDirection.x > 1) sceneDirection.x = 1;
			valid = true;
		}

		if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
		{
			sceneDirection.x--;
			if (sceneDirection.x < 1) sceneDirection.x = -1;
			valid = true;
		}

		if (key == GLFW_KEY_LEFT_SHIFT)
		{
			shiftkeyhelddown = true;
			if (sceneDirection.x != 0)
			{
				animationController->setAnimation("run-forward");
			}
		}		

		if (key == GLFW_KEY_Z) {
			//kickAnim.setAnimation(spriteObj, currentAnim);
			animationController->setAnimation("kick");
		}

		if (!valid) return;


		if (sceneDirection.x == 0)
		{
			animationController->setAnimation("idle");
		}
		else
		{
			animationController->setAnimation("walk-forward");
			if (shiftkeyhelddown) {
				animationController->setAnimation("run-forward");
			}
			spriteObj->transform.direction.x = (sceneDirection.x > 0 || lastDirection.x > 0) ? -1 : 1;
			spriteObj->transform.UpdateMatrix();
		}
	};
	void KeyboardUp(int key) override { 

		bool valid = false;
		glm::ivec2 lastDirection = sceneDirection;
		if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
		{
			sceneDirection.x--;
			valid = true;
		}

		if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
		{
			sceneDirection.x++;
			valid = true;
		}

		if (key == GLFW_KEY_LEFT_SHIFT)
		{
			shiftkeyhelddown = false;
			if (sceneDirection.x != 0) 
			{
				animationController->setAnimation("walk-forward");
			}
		}



		if (key == GLFW_KEY_Q)
		{
			bindDirection = !bindDirection;
		}

		if (!valid) return;

		if (sceneDirection.x == 0)
		{
			animationController->setAnimation("idle");
		}
	};
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
		testSprite->Draw(m_Shader, m_surface);
		background->Draw(m_Shader, m_surface);
	}
	void DrawForeGround() override 
	{
		brickobject->Draw(m_Shader, m_surface);
		spriteObj->Draw(m_Shader, m_surface);
	}
	void DrawDebug() override 
	{
		ImGui::Checkbox("Bind Direction", &bindDirection);
		animationController->Debug();
		background->Debug();
		spriteObj->Debug("Haomaru-sprite wrapper");
		brickobject->Debug();
	}

	//creation
	static Game* Create() { return new ParallaxTest; }
};

static int testIndex = RegisterGame("Scene", "Parallax Test", ParallaxTest::Create);  //do not actually register this lol
