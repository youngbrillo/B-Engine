// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Game.h"
#include "imgui/imgui.h"
#include "Zone.h"
#include "b2BodyGrabber.h"

#include <GLFW/glfw3.h>

// This shows how to use sensor shapes. Sensors don't have collision, but report overlap events.

using namespace SpaceBuster;
class Sensors : public Game
{
public:

	enum
	{
		e_count = 7
	};
	Zone* m_sensor;
	b2Body* m_bodies[e_count];

	glm::vec2 mousePos;
	bool moving;
	b2BodyGrabber* grabber;

	Sensors()
	{
		Game::AppCam->position = glm::vec3(0, 13, 0);
		mousePos = glm::vec2(0);
		moving = false;
		grabber = new b2BodyGrabber(*m_World);
		{
			b2BodyDef bd;
			b2Body* ground = m_World->CreateBody(&bd);

			{
				b2EdgeShape shape;
				shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
				ground->CreateFixture(&shape, 0.0f);
			}

#if 1
			{
				b2FixtureDef sd;
				b2PolygonShape shape;
				shape.SetAsBox(10.0f, 10.0f, b2Vec2(0.0f, 5.0f), 0.0f);
				sd.shape = &shape;
				m_sensor = new Zone(false, b2Vec2(0.0f, 10.0f), 10, 10.0f, 10);//, & sd);

			}
#else
			{
				m_sensor = new Zone(true, b2Vec2(0.0f, 10.0f), 10, 10.0f);
			}
#endif
		}

		{
			b2CircleShape shape;
			shape.m_radius = 1.0f;

			for (int32 i = 0; i < e_count; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-10.0f + 3.0f * i, 20.0f);
				//bd.userData.pointer = i;

				m_bodies[i] = m_World->CreateBody(&bd);
				m_bodies[i]->CreateFixture(&shape, 1.0f);
			}
		}

	}

	// Implement contact listener.
	void onBeginContact(b2Contact* contact) override
	{
		m_sensor->handleBeginContact(contact);
	}

	// Implement contact listener.
	void onEndContact(b2Contact* contact) override
	{
		m_sensor->handleEndContact(contact);
	}

	void DrawForeGround() override
	{
		ImGui::SetNextWindowPos(ImVec2(10.0f, 100.0f));
		ImGui::SetNextWindowSize(ImVec2(200.0f, 60.0f*3));
		ImGui::Begin("Sensor Controls", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		m_sensor->Debug();
		grabber->debug();
		ImGui::SliderFloat2("Mouse pos", &mousePos.x, 0,0);

		ImGui::End();
	}
	
	void DrawDebug() override
	{
		m_physDrawer->renderDebug();
	}

	void FixedUpdate(float dt) override
	{
		m_sensor->FixedUpdate(dt);
	}


	void Update(float dt) override
	{
		//mousePos = Game::AppCam->convertScreenToWorld(mp);
		double x, y;
		glfwGetCursorPos(Game::mainWindow, &x, &y);
		
		glm::vec2 mpw = glm::vec2((float)x, (float)y);
		mousePos = Game::AppCam->convertScreenToWorld(mpw);
		if(moving)
		m_sensor->GetBody()->SetTransform(b2Vec2(mousePos.x, mousePos.y), 0);


	}

	void KeyboardDown(int key)
	{
		if (key == GLFW_KEY_TAB)
		{
			moving = !moving;
		}
	}
	void mouseCallback(GLFWwindow*w, int button, int action, int mode) override
	{
		if (button == GLFW_MOUSE_BUTTON_1)
		{
	
			b2Vec2 _wp(mousePos.x, mousePos.y);
			if (action == GLFW_PRESS)
			{
				grabber->grabRigidBody(_wp);
			}

			if (action == GLFW_RELEASE)
			{
				grabber->releaseRigidBody(_wp);
			}
			if (action == GLFW_RELEASE && mode == GLFW_MOD_SHIFT)
			{
				grabber->toggleRigidBodyType(_wp);
			}
		}
	}

	void mouseCallback_Cursor(GLFWwindow* w, double x, double y) override
	{

		b2Vec2 _wp(mousePos.x, mousePos.y);

		grabber->mouseWorldSpacePosition = _wp;
		grabber->moveRigidBody(_wp);
	}


	static Game* Create()
	{
		return new Sensors;
	}



};

static int testIndex = RegisterGame("Examples", "Sensors", Sensors::Create);
