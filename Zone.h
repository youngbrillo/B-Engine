#pragma once
#ifndef ZONE_H
#define ZONE_H
#include <Box2D/Box2D.h>
#include <vector>
#include <algorithm>

namespace SpaceBuster {

	//The zone's function is to capture objects (dynamic b2bodies) that come within it
	//it is functionally a sensor
	class Zone
	{
	public:
	public:
		Zone(bool isCircle, b2Vec2 position = b2Vec2_zero, float maxCapacity = 5, float xExtent = 10.0f, float yExtent = 10.0f, b2FixtureDef* fixture = nullptr);
		~Zone();

		void handleBeginContact(b2Contact* contact);
		void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		void handleEndContact(b2Contact* contact);
		void FixedUpdate(float dt);
		void Update(float dt);
	private:
		b2Body* m_body;
		b2Fixture* sensor;
		bool isCircle, friendly, release;
		b2Vec2 size, attractionPoint, position;
		int capacity;

		float max_force, min_force, cur_force, deltaForce, releaseTimeStart, releaseTimeCurrent;

		std::vector<b2Body*> touchingBodies;
			b2CircleShape shape;
	public:
		b2Body* GetBody() { return m_body; }
		b2Fixture* GetFixture() { return sensor; }
		b2Shape* GetShape() { return sensor->GetShape(); }
		void Debug(const char* name = nullptr);
		
	protected: 
		void construct(b2FixtureDef* fd);
		void basicConstrution(bool isCircle);
	};
}
#endif // !ZONE_H
