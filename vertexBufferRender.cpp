
#include<glad/glad.h>

#include "Game.h"

#include "imgui.h"
/*
	use this scene to test out more efficient ways to render to the screen!
	expiriment w/ points, lines, line-strips, line_loops, triangles, triangle_strips, and triangle_fans
	until you have mastered the concepts!!
*/

#include "t_Texture.h"
#include "t_Surface.h"
#include "Texture.h"
#include "Transform.h"


class RenderTestScene: public Game
{
public:
	t_Texture * texture;
	t_Surface * surface;
	Shader* shader;

	Transform subject;
	glm::vec4 subjectColor = glm::vec4(1.0f);
	bool cullFaces = false;

	RenderTestScene() : Game()
	{
		//initialize the render pipeline....
		//texture = new t_Texture("./assets/Haohmaru_141.png", true);
		//texture = new t_Texture("./assets/brick.png", true, GL_LINEAR, GL_LINEAR);
		texture = new t_Texture("./assets/his13_000-sheet.png", true);

		
		shader = new Shader("stdsprite.vts", "stdsprite.frs", true);
		surface = new t_Surface(true); //surface->Generate();


		//o
		shader->Use().setInt("image", 0);
		AppCam->zoom = 0.039f;
	};
	//destructor
	~RenderTestScene()
	{
		delete texture;
		delete surface;
		delete shader;
	};

	//key callbacks and frame input key callback
	void KeyboardUp(int key) override { }
	void KeyboardDown(int key)override { }

	void Update(float dt) override { }

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

		shader->Use()
			.SetMatrix4("projection", AppCam->getProjectionMatrix())
			.SetMatrix4("view", AppCam->GetViewMatrix())
			.SetMatrix4("model", subject.m_model)
			.SetVector4f("color", subjectColor);

		texture->Bind();
		surface->Bind();


	} 
	void DrawDebug() override 
	{
		ImGui::ColorEdit4("sub.color", &subjectColor.x);
		ImGui::Checkbox("Face culling", &cullFaces);
		ImGui::Separator();
		texture->Debug();
		surface->Debug("Render target surface");
		subject.Debug("subject's Transform");
	}

	//creation
	static Game* Create() { return new RenderTestScene; }
};

static int testIndex = RegisterGame("Test", "Rendering Methods", RenderTestScene::Create);  
