#include "Game.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Platformer : public Game
{
public:
	enum e_State {
		e_unknown,
		e_above,
		e_below
	};


	Platformer() : Game()
	{

		AppCam->position = glm::vec3(11.75f, 13.25f, 0.0f);
		AppCam->zoom = 1.218f;

		m_World->SetGravity(l_gravity);
		// Ground
		{
			b2BodyDef bd;
			b2Body* ground = m_World->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
			m_ground = ground->CreateFixture(&shape, 0.0f);
		}

		// Platform
		{
			b2BodyDef bd;
			bd.position.Set(0.0f, 10.0f);
			b2Body* body = m_World->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(3.0f, 0.5f);
			m_platform = body->CreateFixture(&shape, 0.0f);

			m_bottom = 10.0f - 0.5f;
			m_top = 10.0f + 0.5f;
		}

		// Actor
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0f, 12.0f);
			b2Body* body = m_World->CreateBody(&bd);

			m_radius = 0.5f;
			b2CircleShape shape;
			shape.m_radius = m_radius;
			m_character = body->CreateFixture(&shape, 20.0f);

			body->SetLinearVelocity(b2Vec2(0, -50.0f));

			m_state = e_unknown;

		}
	}

	void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
	{
		//Test::PreSolve(contact, oldManifold);

		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		if (fixtureA != m_platform && fixtureA != m_character && fixtureA != m_ground)
		{
			return;
		}

		if (fixtureB != m_platform && fixtureB != m_character && fixtureB != m_ground)
		{
			return;
		}

		grounded = true;


		if (fixtureA != m_platform && fixtureA != m_character) return;
		if (fixtureB != m_platform && fixtureB != m_character) return;

		b2Vec2 position = m_character->GetBody()->GetPosition();

		if (position.y < m_top + m_radius - 3.0f * b2_linearSlop)
		{
			
			contact->SetEnabled(false);
		}

		b2Vec2 v = m_character->GetBody()->GetLinearVelocity();
		if (v.y < 0.0f && glfwGetKey(Game::mainWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			contact->SetEnabled(false);
		}
	}
	void onEndContact(b2Contact* contact) override
	{

		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		if (fixtureA != m_platform && fixtureA != m_character && fixtureA != m_ground)
		{
			return;
		}

		if (fixtureB != m_platform && fixtureB != m_character && fixtureB != m_ground)
		{
			return;
		}
		

		grounded = false;
	}
	void FixedUpdate(float dt) override
	{

		//g_debugDraw.DrawString(5, m_textLine, "Character Linear Velocity: %f", v.y);
		//m_textLine += m_textIncrement;
		if (!grounded)
			plat_direction.y = 0;

	}
	void KeyboardDown(int key) override
	{
		switch (key)
		{
			case GLFW_KEY_W: //UP
			case GLFW_KEY_SPACE: //JUMP
				if(grounded) plat_direction.y = 1;
				break;
			case GLFW_KEY_S: //DOWN
				plat_direction.y = -1;
				break;
			case GLFW_KEY_A: //LEFT
				plat_direction.x = -1;
				break;
			case GLFW_KEY_D: //RIGHT
				plat_direction.x = 1;
				break;
			default:
				break;
		}
		plat_velocity = b2Vec2(plat_direction.x * maxVelocity, plat_direction.y * maxVelocity * 2);
		m_character->GetBody()->SetLinearVelocity(plat_velocity);

	}
	void KeyboardUp(int key) override
	{

		switch (key)
		{
		case GLFW_KEY_W: //UP
		case GLFW_KEY_S: //DOWN
		case GLFW_KEY_SPACE: //JUMP
			plat_direction.y = 0;
			break;
		case GLFW_KEY_A: //LEFT
		case GLFW_KEY_D: //RIGHT
			plat_direction.x = 0;
			break;
		default:
			break;
		}
		//plat_velocity = b2Vec2(plat_direction.x * maxVelocity, plat_direction.y * maxVelocity);
	
	}

	void DrawDebug() override
	{
		ImGui::SliderFloat2("Char.Linear Velocity", &plat_velocity.x, 0, 0);
		ImGui::SliderFloat2("Char.Direction", &plat_direction.x, 0, 0);
		ImGui::SliderFloat("Char.maxSpeed", &maxVelocity, 0, 0);

		if(ImGui::SliderFloat2("gravity", &l_gravity.x, -50.f, 50.f))
			m_World->SetGravity(l_gravity);

		ImGui::Checkbox("On ground?", &grounded);

	}
	static Game* Create()
	{
		return new Platformer;
	}

	float m_radius, m_top, m_bottom;
	e_State m_state;
	b2Fixture* m_ground;
	b2Fixture* m_platform;
	b2Fixture* m_character;

	//movement control utils
	b2Vec2 plat_direction = b2Vec2_zero;
	b2Vec2 plat_velocity = b2Vec2_zero;
	b2Vec2 l_gravity = b2Vec2(0.0f, -10.0f * 3);
	float maxVelocity = 10.0f;

	bool grounded = false;
};

static int testIndex = RegisterGame("Examples", "Platformer", Platformer::Create);