#pragma once
#ifndef ASTROID_H
#define ASTROID_H

#include <Box2D/Box2D.h>
#include <vector>
#include "Utility_Physics.h"

namespace SpaceBuster {

	class Astroid
	{
	protected:
		enum astroid_class {
			small = 0,
			medium = 1,
			large  = 2,
		};
	public:
		Astroid(const b2Vec2& startPos, const b2Vec2& startLinearVel, const float& startAngVel, b2Vec2 size = b2Vec2(1.0f, 1.0f));
		~Astroid();
		void Update(float dt);
		void handleCollision(b2Contact* contact, const b2Manifold* oldManifold);
		void handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
		void Debug(const char* title = nullptr);
	private:
		b2Fixture* collider;
		b2Body* body;
		float health, expireTimeOut, expireTimeLeft;
		bool broken, deleteFlag, fragmentFlag;
		b2Vec2 size;
		astroid_class astroid_class_type;
		collisionType_astroids collisionType;
		friend class AstroidFactory;
		friend class Zone;
	};
}




#endif // !ASTROID_H

