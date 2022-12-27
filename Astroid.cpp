#include "Astroid.h"
#include "Game.h"

using namespace SpaceBuster;

SpaceBuster::Astroid::Astroid(const b2Vec2& startPos, const b2Vec2& startLinearVel, const float& startAngVel, b2Vec2 _size)
	: health(100.0f), broken(false), deleteFlag(false)
	, expireTimeOut(.5f), expireTimeLeft(0.0f)
	, size(_size)
	, fragmentFlag(false)
	, astroid_class_type(astroid_class::small)
	, collisionType(collisionType_astroids::collision_astroid)
{

	b2PolygonShape shape;
	//b2CircleShape shape;

	if (size.x < 1) {
		size.x = 1.0f;
		size.y = 1.0f;
	}
	shape.SetAsBox(size.x/2.0f, size.y/2.0f);
	//shape.m_radius = (size.x / 2.0f);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;
	fd.friction = 0.3f;
	fd.restitution = .40f;
	
	b2BodyDef bd;
	bd.type = b2_dynamicBody;

	bd.position.Set(startPos.x, startPos.y);
	body = Game::m_World->CreateBody(&bd);

	collider = body->CreateFixture(&fd);
	collider->SetUserData(&collisionType);
	//attachFrictionJoint( body);


	body->SetLinearVelocity(startLinearVel);
	body->SetAngularVelocity(startAngVel);

	//set class based on size
	float _rad = size.x;

	if (_rad >= 2.5f)
	{
		astroid_class_type = astroid_class::medium;
		if (_rad >= 5.0f) {
			astroid_class_type = astroid_class::large;
		}
	}
	//printf("Creating astroid of size: %.2f, %.2f,  %d\n", size.x, _rad, astroid_class_type);
}

SpaceBuster::Astroid::~Astroid()
{
	body->DestroyFixture(collider);
	collider = NULL;
	body->GetWorld()->DestroyBody(body);
	body = NULL;
}

void SpaceBuster::Astroid::Update(float dt)
{

	if (broken)
	{
		expireTimeLeft += dt;
		if (expireTimeLeft >= expireTimeOut)
		{
			deleteFlag = true;
		}
	}

}

void SpaceBuster::Astroid::handleCollision(b2Contact* contact, const b2Manifold* oldManifold)
{
	return;
	b2Fixture* A = contact->GetFixtureA();
	b2Fixture* B = contact->GetFixtureB();

	if (A != collider && B != collider) return;

//	printf("Astroid here! I'm collidin' with somthin!!!\n");

}

void SpaceBuster::Astroid::handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	if (broken) return;
	b2Fixture* A = contact->GetFixtureA();
	b2Fixture* B = contact->GetFixtureB();
	b2Fixture* other;

	if (A != collider && B != collider) return;

	if (A == collider) { other = B; }
	else { other = A; }

	collisionType_astroids* ct = (collisionType_astroids*)other->GetUserData();
	bool canTrackCollision = !!ct;

	if (!canTrackCollision) return;
	
	// Should the body break?
	int32 count = contact->GetManifold()->pointCount;

	float maxImpulse = 0.0f;
	for (int32 i = 0; i < count; ++i)
	{
		maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
	}

	float DM = collider->GetDensity() * body->GetMass();
	if (maxImpulse > DM * 40.0f)
	{
		// Flag the body for breaking.
		health -= 100.0f;
	}
	else //then subtract from astroids health dir proportional to above comparison
	{
		if (*ct == collisionType_astroids::collision_bullet)
		{
			health -= maxImpulse / (DM * 40.0f) * 100;
		}
	}

	//printf("Astroid post solve:: impact for: (%.2f/%.2f.) health = %.2f.\n", maxImpulse, DM * 40.0f, health);


	if (health <= 0.0f) {
		if (astroid_class_type == astroid_class::small)
			broken = true;
		else
			fragmentFlag = true;


		//printf("Astroid is breaking: %s, is fragmenting: %s\n", broken ? "yes" : "No", fragmentFlag ? "Yes" : "No");
	}
}

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


void SpaceBuster::Astroid::Debug(const char* title)
{
	std::string vtitle = title != nullptr ? title : "Unknown astroid";
	
	if (ImGui::TreeNode(vtitle.c_str()))
	{
		ImGui::Checkbox("Broken status", &broken);
		ImGui::Checkbox("Delete status", &deleteFlag);

		ImGui::SliderFloat("health", &health, 0, 100.0f);

		ImGui::SliderFloat("destroy timer", &expireTimeLeft, 0.0f, expireTimeOut);
		ImGui::SliderFloat("timer upperlimit", &expireTimeLeft, 0.0f, 2.0f);

		ImGui::TreePop();

	}
}
