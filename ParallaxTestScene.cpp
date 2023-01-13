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
	void flipDirectionHorizontal(bool value)
	{
		flipHorizontal = value;
		spritedir.x = flipHorizontal ? -1.0f : 1.0f;
	}
};


struct simpleCharaAnimator
{
	simpleCharaAnimator() : animationBounds(0, 0) {};
	simpleCharaAnimator(glm::ivec2 b, bool singleUse = false) : animationBounds(b), doOnce(singleUse) {};
	simpleCharaAnimator(int a, int b, bool singleUse = false) : animationBounds(a, b), doOnce(singleUse) {};

	void setAnimation(Sprite* spritetoanimate, simpleCharaAnimator* nosey)
	{
		spritetoanimate->animStartIndex = animationBounds.x;
		spritetoanimate->animEndIndex = animationBounds.y;
		spritetoanimate->index = animationBounds.x;
		spritetoanimate->animate = true;

		//if (nosey != nullptr)
		//{
		//	nosey = this;
		//	if (nosey->lastAnimation != this)
		//	{
		//		nosey->lastAnimation = this;
		//	}
		//}
		

	}

	simpleCharaAnimator* lastAnimation = nullptr;

	glm::ivec2 animationBounds;

	bool doOnce = false; 
};

class ParallaxTest : public Game
{
public:
	//constructor
	spriteWrapper* spriteObj;
	Texture* foregroundTexture, * bg01, * bg02, * bg03, * bg04, * bg05, * bg06;
	GameObject* brickobject;

	ScrollingSprite* background;


	glm::ivec2 sceneDirection;
	simpleCharaAnimator runningAnim, idleAnim, walkAnim, walkBackAnim, kickAnim, *currentAnim;
	bool shiftkeyhelddown = false;
	ParallaxTest() : Game()
	{
		//load textures
		foregroundTexture = new Texture("./assets/Haohmaru_141.png", true);
		bg01 = new Texture("./assets/bgs/foreground_00.png", true);
		bg02 = new Texture("./assets/bgs/foreground_01.png", true);
		bg03 = new Texture("./assets/bgs/foreground_02.png", true);
		bg04 = ResourceManager::GetTexturePtr("icon");//new Texture("./assets/bgs/foreground_04.png", true);
		bg05 = ResourceManager::GetTexturePtr("SparePart");//new Texture("./assets/bgs/background_settingSun.png", true);
		bg06 = ResourceManager::GetTexturePtr("Ammo");//new Texture("./assets/bgs/foreground_05.png", true);

		//set up background
		ScrollingSpriteDefinition ssd;
		//ssd.layerCount = 6;
		//ssd.layers.emplace_back(bg06);
		//ssd.layers.emplace_back(bg05);
		//ssd.layers.emplace_back(bg04);
		ssd.layers.emplace_back(bg03);
		ssd.layers.emplace_back(bg02);
		ssd.layers.emplace_back(bg01);

		ssd.position = glm::vec3(-10.0f, 0.0f, 0.0f);
		ssd.pivot = glm::vec3(-0.5f, -0.5, 0.0f);
		ssd.size = glm::vec3(3.784f, 1.0f, 1.0f);
		ssd.scale = 7.459f;

		background = new ScrollingSprite(&ssd);

		//set scene direction to 
		sceneDirection = glm::ivec2(0, 0);
		//set animations (for flavor)
		runningAnim = simpleCharaAnimator(87, 92);
		idleAnim = simpleCharaAnimator(0, 9);
		walkAnim = simpleCharaAnimator(26, 33);
		walkBackAnim = simpleCharaAnimator(34, 41);
		kickAnim = simpleCharaAnimator(291, 300, true);
		{
			//intialize foreground elements
			spriteObj = new spriteWrapper(foregroundTexture, 23, 23);
			//configure the size to be comparable w/ a normal game object of size 1,1,1
			spriteObj->size = glm::vec3(3.0f, 3.0f, 1.0f);
			spriteObj->pivot = glm::vec3(0.0, -0.407f, 0.0f);
			//set up initial animation frame
			//spriteObj->sprite->animStartIndex = 87;
			//spriteObj->sprite->animEndIndex = 92;
			//spriteObj->sprite->map->GetFrame(0);
			spriteObj->sprite->animate = true;

			idleAnim.setAnimation(spriteObj->sprite, currentAnim);

		}
		//assign animations


		//change shader
		m_Shader = new Shader("stdsprite.vts", "stdsprite.frs", true);
		//reconfig cam

		AppCam->zoom = 0.112f;
		if (AppCam->Width < 1000 && AppCam->Height < 500)
		{
			AppCam->zoom = 0.29f;
		}
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
		delete brickobject;
		delete background;

		//delete bg04;
		//delete bg05;
		//delete bg06;
		delete foregroundTexture;
		//delete BackgroundTexture01;
		//delete BackgroundTexture02;
		//delete BackgroundTexture03;


	};
	//mouse callbacks
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override {}
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height) override {}

	void Update(float dt) override {
		spriteObj->Update(dt);
		//background->direction = this->sceneDirection;
		background->Update(dt);
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
				runningAnim.setAnimation(spriteObj->sprite, currentAnim);
			}
		}		

		if (key == GLFW_KEY_Z) {
			kickAnim.setAnimation(spriteObj->sprite, currentAnim);
		}

		if (!valid) return;


		if (sceneDirection.x == 0)
		{
			idleAnim.setAnimation(spriteObj->sprite, currentAnim);
		}
		else
		{
			//if (abs(sceneDirection.x) > 1)
			//{
			//	runningAnim.setAnimation(spriteObj->sprite);
			//	if (abs(lastDirection.x) == 2 && (abs(sceneDirection.x) == 1))
			//	{
			//		//don't just walk, walk backward
			//		walkBackAnim.setAnimation(spriteObj->sprite);
			//	}
			//}
			//else 
			//{
			//	walkAnim.setAnimation(spriteObj->sprite);
			//}


			walkAnim.setAnimation(spriteObj->sprite, currentAnim);
			if(shiftkeyhelddown)
				runningAnim.setAnimation(spriteObj->sprite, currentAnim);

			spriteObj->flipDirectionHorizontal(sceneDirection.x > 0 || lastDirection.x > 0);
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
				walkAnim.setAnimation(spriteObj->sprite, currentAnim);
		}
		if (!valid) return;

		if (sceneDirection.x == 0)
		{
			idleAnim.setAnimation(spriteObj->sprite, currentAnim);
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
		background->Draw(m_Shader);

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
