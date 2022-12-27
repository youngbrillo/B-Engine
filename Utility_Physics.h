#pragma once

#ifndef UTILITY_PHYSICS_H
#define UTILITY_PHYSICS_H

#include <Box2D/Box2D.h>

b2Body* generateBounds(b2World* world, b2Vec2 pos = b2Vec2(0, 0), float max = 10.0f, float min = 5.0f, float bounce = 0.0f, float density = 0.0f, float friction = 0.0f);
b2Body* generateBounds_rect(b2World* world, b2Vec2 pos = b2Vec2(0,0), float max = 10.0f, float min = 5.0f, float bounce = 0.0f, float density = 0.0f, float friction = 0.0f);
void regenerateBounds(b2Body* body, float max = 10.0f, float min = 5.0f, float bounce = 0.0f, float density = 0.0f, float friction = 0.0f);

//for top down scenes when you don't want everything having infinite inertia all the time
void attachFrictionJoint(b2Body* ground, b2Body* subject);
b2Joint* attachFrictionJoint2(b2Body* ground, b2Body* subject, float gravity = 10.0f, float maxForceDelta = 0.5f, float maxTorqueDelta = 0.2f);


void createRectShape(b2Body* body, float width, float height);
void createOctShape(b2Body* body, float width, float height);

enum class collisionType_astroids {
	collision_unknown = 0,
	collision_bullet = 1 ,
	collision_astroid = 2,
	collision_ship    = 3,
	collision_powerup = 4,
	collision_material= 5
};


void DrawString(const int x, const int y, const char* string, ...);
void DrawString(const int x, const int y, const char* string, float r, float g, float b, float a, ...);
void DrawString(const b2Vec2& worldPoint, const char* string, ...);
#endif // !UTILITY_PHYSICS_H
