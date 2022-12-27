#include "Utility_Physics.h"
#include <vector>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


b2Body* generateBounds(b2World* world, b2Vec2 pos, float max, float min , float bounce, float density, float friction)
{
	b2BodyDef bd;
	bd.position = pos;
	b2Body* body = world->CreateBody(&bd);

	regenerateBounds(body, max, min, bounce, density, friction);
	return body;
}

b2Body* generateBounds_rect(b2World* world, b2Vec2 pos, float x, float y, float bounce, float density, float friction)
{

	b2BodyDef bd;
	bd.position = pos;
	b2Body* body = world->CreateBody(&bd);



	b2EdgeShape shape;

	b2FixtureDef sd;
	sd.shape = &shape;
	sd.density = density;
	sd.restitution = bounce;
	sd.friction = friction;

	body->CreateFixture(&sd);
	// Left vertical
	shape.Set(b2Vec2(-x, -y), b2Vec2(-x, y));
	body->CreateFixture(&sd);

	// Right vertical
	shape.Set(b2Vec2(x, -y), b2Vec2(x, y));
	body->CreateFixture(&sd);


	// Top horizontal
	shape.Set(b2Vec2(-x, y), b2Vec2(x, y));
	body->CreateFixture(&sd);

	// Bottom horizontal
	shape.Set(b2Vec2(-x, -y), b2Vec2(x, -y));
	body->CreateFixture(&sd);


	return body;
}

//to do get all fixtures and delete them from the body
void regenerateBounds(b2Body* body, float max, float min, float bounce, float density, float friction)
{
	b2EdgeShape shape;

	b2FixtureDef sd;
	sd.shape = &shape;
	sd.density = density;
	sd.restitution = bounce;
	sd.friction = friction;

	// Left vertical
	shape.Set(b2Vec2(-max, -min), b2Vec2(-max, min));
	body->CreateFixture(&sd);

	// Right vertical
	shape.Set(b2Vec2(max, -min), b2Vec2(max, min));
	body->CreateFixture(&sd);


	// Top horizontal
	shape.Set(b2Vec2(-min, max), b2Vec2(min, max));
	body->CreateFixture(&sd);

	// Bottom horizontal
	shape.Set(b2Vec2(-min, -max), b2Vec2(min, -max));
	body->CreateFixture(&sd);

	//Top - Right connection
	shape.Set(b2Vec2(min, max), b2Vec2(max, min));
	body->CreateFixture(&sd);

	//Top - left connection
	shape.Set(b2Vec2(-min, max), b2Vec2(-max, min));
	body->CreateFixture(&sd);

	//bottom - left connection
	shape.Set(b2Vec2(-min, -max), b2Vec2(-max, -min));
	body->CreateFixture(&sd);

	//bottom - right connection
	shape.Set(b2Vec2(min, -max), b2Vec2(max, -min));
	body->CreateFixture(&sd);


}

void attachFrictionJoint(b2Body* ground, b2Body* subject)
{
	float gravity = 10.0f;
	float I = subject->GetInertia();
	float mass = subject->GetMass();

	// Compute an effective radius that can be used to
	// set the max torque for a friction joint
	// For a circle: I = 0.5 * m * r * r ==> r = sqrt(2 * I / m)
	float radius = b2Sqrt(2.0f * I / mass);

	b2FrictionJointDef jd;
	jd.bodyA = ground;
	jd.bodyB = subject;
	jd.localAnchorA.SetZero();
	jd.localAnchorB = subject->GetLocalCenter();
	jd.collideConnected = true;
	jd.maxForce = 0.5f * mass * gravity;
	jd.maxTorque = 0.2f * mass * radius * gravity;

	subject->GetWorld()->CreateJoint(&jd);
}

b2Joint* attachFrictionJoint2(b2Body* ground, b2Body* subject, float gravity, float maxForceDelta, float maxTorqueDelta)
{
	float I = subject->GetInertia();
	float mass = subject->GetMass();

	// Compute an effective radius that can be used to
	// set the max torque for a friction joint
	// For a circle: I = 0.5 * m * r * r ==> r = sqrt(2 * I / m)
	float radius = b2Sqrt(2.0f * I / mass);

	b2FrictionJointDef jd;
	jd.bodyA = ground;
	jd.bodyB = subject;
	jd.localAnchorA.SetZero();
	jd.localAnchorB = subject->GetLocalCenter();
	jd.collideConnected = true;
	jd.maxForce = maxForceDelta * mass * gravity;
	jd.maxTorque = maxTorqueDelta * mass * radius * gravity;

	return subject->GetWorld()->CreateJoint(&jd);
}

void createRectShape(b2Body* body, float width, float height)
{
}

void createOctShape(b2Body* body, float width, float height)
{
}

void DrawString(const int x, const int y, const char* string, ...)
{
	va_list arg;
	__crt_va_start(arg, string);
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar;

	flags = flags | ImGuiWindowFlags_NoBackground;
 
	ImGui::Begin("Overlay",  NULL, flags);
	ImGui::SetCursorPos(ImVec2(float(x), float(y)));
	ImGui::TextColoredV(ImColor(230, 153, 153, 255), string, arg);
	ImGui::End();

	
	__crt_va_end(arg);
}

void DrawString(const int x, const int y, const char* string, float r, float g, float b, float a, ...)
{
	va_list arg;
	__crt_va_start(arg, string);
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar;

	flags = flags | ImGuiWindowFlags_NoBackground;

	ImGui::Begin("Overlay", NULL, flags);
	ImGui::SetCursorPos(ImVec2(float(x), float(y)));
	ImGui::TextColoredV(ImColor(r, g, b, a), string, arg);
	ImGui::End();
	__crt_va_end(arg);
}

void DrawString(const b2Vec2& worldPoint, const char* string, ...)
{
}
