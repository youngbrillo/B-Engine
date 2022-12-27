#include "b2BodyGrabber.h"

b2BodyGrabber::b2BodyGrabber(b2World& _worldRef)
	: World(&_worldRef), mouseJoint(nullptr), grabEnabled(true)
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(0.0f, 0.0f);
	groundBody = World->CreateBody(&bodyDef);
}
b2BodyGrabber::~b2BodyGrabber()
{
}

void b2BodyGrabber::grabRigidBody(b2Vec2& worldSpacePos)
{
	mouseWorldSpacePosition = worldSpacePos;

	if (mouseJoint != NULL)
	{
		return;
	}

	// Make a small box.
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = worldSpacePos - d;
	aabb.upperBound = worldSpacePos + d;

	// Query the world for overlapping shapes.
	EditorQueryCallback_DynamicBody callback(worldSpacePos);
	World->QueryAABB(&callback, aabb);

	if (callback.m_fixture)
	{
		float frequencyHz = 5.0f;
		float dampingRatio = 0.7f;

		b2Body* body = callback.m_fixture->GetBody();
		b2MouseJointDef jd;
		jd.bodyA = groundBody;
		jd.bodyB = body;
		jd.target = worldSpacePos;
		jd.maxForce = 1000.0f * body->GetMass();

		jd.dampingRatio = dampingRatio;

		//b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);

		mouseJoint = (b2MouseJoint*)World->CreateJoint(&jd);
		body->SetAwake(true);

	//	printf("Grabbed fixture successfully @ (%.2f, %.2f)\n", worldSpacePos.x, worldSpacePos.y);
		grabEnabled = true;
	}
	else
	{
		grabEnabled = false;
	}
}

void b2BodyGrabber::moveRigidBody(b2Vec2& p)
{
	mouseWorldSpacePosition = p;
	if (mouseJoint)
	{
		mouseJoint->SetTarget(p);
	}
}

void b2BodyGrabber::releaseRigidBody(b2Vec2& worldSpacePos)
{
	if (mouseJoint && grabEnabled)
	{
		World->DestroyJoint(mouseJoint);
		mouseJoint = NULL;
		grabEnabled = false;
	//	printf("Released fixture successfully @ (%.2f, %.2f)\n", worldSpacePos.x, worldSpacePos.y);
	}
}

void b2BodyGrabber::toggleRigidBodyType(b2Vec2& worldSpacePos)
{
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = worldSpacePos - d;
	aabb.upperBound = worldSpacePos + d;

	// Query the world for overlapping shapes.
	EditorQueryCallback_AnyBody callback(worldSpacePos);
	World->QueryAABB(&callback, aabb);

	if (callback.m_fixture)
	{
		b2Body* body = callback.m_fixture->GetBody();
		switch (body->GetType())
		{
		case b2BodyType::b2_dynamicBody:
			body->SetType(b2_kinematicBody); break;
		case b2BodyType::b2_kinematicBody:
			body->SetType(b2_staticBody); break;
		case b2BodyType::b2_staticBody:
			body->SetType(b2_dynamicBody); break;
		default:
			break;
		}
	}
}

#include "imgui.h"

void b2BodyGrabber::debug()
{

	if (ImGui::TreeNode("Editor")) {
		ImGui::Checkbox("Grab enabled?", &grabEnabled);
		ImGui::SliderFloat2("mouse position", &mouseWorldSpacePosition.x, 0, 0);

		ImGui::TreePop();
	}
}
