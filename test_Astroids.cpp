
#include "Game.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <math.h>

#include "Bullet.h"

class Astroids : public Game
{
	
public:

	bool followShip = false;

	Astroids() : Game(), m_bomb(nullptr), mousePosition(0),  playerMouseAngle(0.0f)
	{
		m_World->SetGravity(b2Vec2(0.0f, 0.0f));

		const float k_restitution = 0.4f;

		AppCam->position = glm::vec3(0.0f, 20.0f, 0.0f);
		AppCam->zoom = 1.772f;


		
		b2Body* ground;
		{
			b2BodyDef bd;
			bd.position.Set(0.0f, 20.0f);
			ground = m_World->CreateBody(&bd);

			b2EdgeShape shape;

			b2FixtureDef sd;
			sd.shape = &shape;
			sd.density = 0.0f;
			sd.restitution = k_restitution;

			// Left vertical
			shape.Set(b2Vec2(-20.0f, -20.0f), b2Vec2(-20.0f, 20.0f));
			ground->CreateFixture(&sd);

			// Right vertical
			shape.Set(b2Vec2(20.0f, -20.0f), b2Vec2(20.0f, 20.0f));
			ground->CreateFixture(&sd);

			// Top horizontal
			shape.Set(b2Vec2(-20.0f, 20.0f), b2Vec2(20.0f, 20.0f));
			ground->CreateFixture(&sd);

			// Bottom horizontal
			shape.Set(b2Vec2(-20.0f, -20.0f), b2Vec2(20.0f, -20.0f));
			ground->CreateFixture(&sd);
		}

		{
			b2Transform xf1;
			xf1.q.Set(0.3524f * b2_pi);
			xf1.p = xf1.q.GetXAxis();

			b2Vec2 vertices[3];
			vertices[0] = b2Mul(xf1, b2Vec2(-1.0f, 0.0f));
			vertices[1] = b2Mul(xf1, b2Vec2(1.0f, 0.0f));
			vertices[2] = b2Mul(xf1, b2Vec2(0.0f, 0.5f));

			b2PolygonShape poly1;
			poly1.Set(vertices, 3);

			b2FixtureDef sd1;
			sd1.shape = &poly1;
			sd1.density = 2.0f;

			b2Transform xf2;
			xf2.q.Set(-0.3524f * b2_pi);
			xf2.p = -xf2.q.GetXAxis();

			vertices[0] = b2Mul(xf2, b2Vec2(-1.0f, 0.0f));
			vertices[1] = b2Mul(xf2, b2Vec2(1.0f, 0.0f));
			vertices[2] = b2Mul(xf2, b2Vec2(0.0f, 0.5f));

			b2PolygonShape poly2;
			poly2.Set(vertices, 3);

			b2FixtureDef sd2;
			sd2.shape = &poly2;
			sd2.density = 2.0f;

			b2BodyDef bd;
			bd.type = b2_dynamicBody;

			bd.position.Set(0.0f, 3.0);
			bd.angle = b2_pi;
			bd.allowSleep = false;
			m_body = m_World->CreateBody(&bd);
			m_body->CreateFixture(&sd1);
			m_body->CreateFixture(&sd2);

			float gravity = 10.0f;
			float I = m_body->GetInertia();
			float mass = m_body->GetMass();

			// Compute an effective radius that can be used to
			// set the max torque for a friction joint
			// For a circle: I = 0.5 * m * r * r ==> r = sqrt(2 * I / m)
			float radius = b2Sqrt(2.0f * I / mass);

			b2FrictionJointDef jd;
			jd.bodyA = ground;
			jd.bodyB = m_body;
			jd.localAnchorA.SetZero();
			jd.localAnchorB = m_body->GetLocalCenter();
			jd.collideConnected = true;
			jd.maxForce = 0.5f * mass * gravity;
			jd.maxTorque = 0.2f * mass * radius * gravity;

			m_World->CreateJoint(&jd);
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 0.5f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;
			fd.friction = 0.3f;

			for (int i = 0; i < 10; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;

				bd.position.Set(0.0f, 7.0f + 1.54f * i);
				b2Body* body = m_World->CreateBody(&bd);

				body->CreateFixture(&fd);

				float gravity = 10.0f;
				float I = body->GetInertia();
				float mass = body->GetMass();

				// For a circle: I = 0.5 * m * r * r ==> r = sqrt(2 * I / m)
				float radius = b2Sqrt(2.0f * I / mass);

				b2FrictionJointDef jd;
				jd.localAnchorA.SetZero();
				jd.localAnchorB.SetZero();
				jd.bodyA = ground;
				jd.bodyB = body;
				jd.collideConnected = true;
				jd.maxForce = mass * gravity;
				jd.maxTorque = 0.1f * mass * radius * gravity;

				m_World->CreateJoint(&jd);
			}
		}


		forwardForce = b2Vec2(0.0f, -50.0f);
	}



	void FixedUpdate(float deltaTime) override
	{
		//g_debugDraw.DrawString(5, m_textLine, "Forward (W), Turn (A) and (D)");
		//m_textLine += m_textIncrement;
		if (glfwGetKey(Game::mainWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			b2Vec2 f = m_body->GetWorldVector(forwardForce);
			b2Vec2 p = m_body->GetWorldPoint(b2Vec2(0.0f, 3.0f));

			//p = b2Vec2(- std::cos(playerMouseAngle) * 10, std::sin(playerMouseAngle) * 10);
			m_body->ApplyForce(f, p, true);
		}

		if (glfwGetKey(Game::mainWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			b2Vec2 f = m_body->GetWorldVector(-forwardForce);
			b2Vec2 p = m_body->GetWorldPoint(b2Vec2(0.0f, 3.0f));
			m_body->ApplyForce(f, p, true);
		}

		if (glfwGetKey(Game::mainWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_body->ApplyTorque(torqueMag, true);
		}

		if (glfwGetKey(Game::mainWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_body->ApplyTorque(-torqueMag, true);
		}


		if (glfwGetKey(Game::mainWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			b2Vec2 p = m_body->GetWorldPoint(bombLaunchPoint);
			//velocity should be in the direction the player is facing
			//with a fairly large magnitudew
			b2Vec2 v = m_body->GetWorldVector(bombVelocity);

			LaunchBomb(p, v);

		}
		if(false)
		{
			//m_body->SetTransform(m_body->GetPosition(), playerMouseAngle);
			// pivotOffset is the displacement vector pointing from the turret body origin to the pivot point
			// get the world co-ordinates of the pivot point
			b2Vec2 cursorPos = m_body->GetPosition();

			// get the world co-ordinates of the cursor position
			b2Vec2 turretPivot = b2Vec2(mousePosition.x, mousePosition.y);

			// get the displacement vector between the cursor and the turret pivot
			float displacement = atan2f(cursorPos.y - turretPivot.y, cursorPos.x - turretPivot.x);

			// set the angle of the turret body to the angle of the displacement vector
			m_body->SetTransform(m_body->GetPosition(), displacement);
		}
	}
	float playerMouseAngle;
	void Update(float dt) override
	{
		if (m_bomb) {
			m_bomb->Update(dt);
		}


		if(followShip)
			AppCam->position = glm::vec3(m_body->GetPosition().x, m_body->GetPosition().y, 0.0f);



		//get mouse position in world coords
		{
			double x, y;
			glfwGetCursorPos(Game::mainWindow, &x, &y);
			glm::vec2 mp((float)x, (float)y);

			mousePosition = AppCam->convertScreenToWorld(mp);
			playerMouseAngle = atan2(m_body->GetPosition().y - mousePosition.y, m_body->GetPosition().x - mousePosition.x);

		}
		forcePoint = m_body->GetWorldVector(forwardForce);

		//playerMouseAngle = glm::degrees(playerMouseAngle);

		if (m_bomb)
		{
			if (m_bomb->canDestroy()) {

				delete m_bomb;
				m_bomb = NULL;
			}
		}
	}
	void DrawForeGround() override
	{
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(mousePosition, 0));
		model = glm::scale(model, glm::vec3(1) * 0.5f);

		m_Shader->Use().SetMatrix4("model", model).SetVector4f("color", glm::vec4(0, 1, 0.5, 1));

		m_surface->Bind();
	}
	void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
	{
		if (!m_bomb) return;
		b2Body* A = contact->GetFixtureA()->GetBody();
		b2Body* B = contact->GetFixtureB()->GetBody();

		if (A != m_bomb->getBody() && B != m_bomb->getBody()) return;

		//bomb has collided with something

		m_bomb->setContact(true);
	}

	void LaunchBomb(const b2Vec2& position, const b2Vec2& velocity)
	{
		if (m_bomb)
		{
			return;
		}
		m_bomb = new SpaceBuster::Bullet(position, velocity);
	}

	//player params
	b2Body* m_body;
	b2Vec2 forwardForce;// = b2Vec2(0.0f, -50.0f);
	float torqueMag = 10.0f;

	//projectile params
	SpaceBuster::Bullet* m_bomb;

	b2Vec2 bombLaunchPoint = b2Vec2(0.0f, -0.5f);
	b2Vec2 bombVelocity = b2Vec2(0.0f, -50.0f);
	//mouse params - to rotate player
	glm::vec2 mousePosition;
	b2Vec2 forcePoint;

	void DrawDebug() override {
		ImGui::SliderFloat2("Mouse Point", &mousePosition.x, -5, 5);
		ImGui::SliderFloat2("force Point", &forcePoint.x, 0, 0);
		ImGui::SliderFloat("diff ", &playerMouseAngle, 0, 0);


		ImGui::Checkbox("Follow Player", &followShip);
		ImGui::SliderFloat2("Forward Force", &forwardForce.x, -50.0f, 50.0f);


		ImGui::SliderFloat("Torque Magnitude", &torqueMag, 0.0f, 10.0f);

		if (ImGui::TreeNode("Projectile"))
		{
			//if(m_bomb)
				//ImGui::Checkbox("Destroy Bomb", &m_bomb->setContact();
			ImGui::SliderFloat2("bomb position", &bombLaunchPoint.x, -10, 10.0f);
			ImGui::SliderFloat2("bomb Force", &bombVelocity.x, -50.0f, 50.0f);

			ImGui::TreePop();
		}


	}


	static Game* Create() { return new Astroids; }
};

static int testIndex = RegisterGame("Examples", "Astroids", Astroids::Create);