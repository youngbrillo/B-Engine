#pragma once
#ifndef B2BODYGRABBER_H
#define B2BODYGRABBER_H

#include <Box2D/Box2D.h>

class b2BodyGrabber
{
public:
	b2BodyGrabber(b2World& _worldRef);
	~b2BodyGrabber();

	void grabRigidBody(b2Vec2& worldSpacePos);
	void moveRigidBody(b2Vec2& p);
	void releaseRigidBody(b2Vec2& worldSpacePos);
	void toggleRigidBodyType(b2Vec2& worldSpacePos);


	//
	void debug();


	bool grabEnabled;
private:
	b2World* World;

	b2MouseJoint* mouseJoint;
public:
	b2Vec2			mouseWorldSpacePosition;
	b2Body* groundBody;
};



class EditorQueryCallback_DynamicBody : public b2QueryCallback
{
public:
	EditorQueryCallback_DynamicBody(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture) override
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};

class EditorQueryCallback_AnyBody : public b2QueryCallback
{
public:
	EditorQueryCallback_AnyBody(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture) override
	{
		b2Body* body = fixture->GetBody();
		if (body)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};
#endif // !B2BODYGRABBER_h

