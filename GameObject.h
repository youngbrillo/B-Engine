#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include "Surface.h"
#include "Texture.h"
#include "Shader.h"

typedef struct  GameObjectDefinition {
	std::string		name;

	glm::vec3		position = glm::vec3(1.0f),
					size = glm::vec3(1.0f),
					orientation_axis = glm::vec3(0, 0, 1.0f),
					rotation_axis = glm::vec3(0, 0, 1.0f);

	glm::vec4		color = glm::vec4(1.0f);

	float			scale = 1.0f,
					orientation = 0.0f,
					rotation = 0.0f,
					angularVelocity = 0.0f;

	bool			enabled = true,
					enabled_physics = false,
					visible = true,
					setBodyType = false,
					isSensor = false;

	b2BodyDef*		objBodydef = NULL;
	b2FixtureDef*	objFixDef = NULL;
	b2Vec2			linearVelocity = b2Vec2_zero;
	Texture*		texture = NULL;

	b2BodyType		bodyType = b2BodyType::b2_dynamicBody;
};

class GameObject
{
public:
	GameObject(GameObjectDefinition* def);
	virtual ~GameObject();

	virtual void FixedUpdate(float dt);
	virtual void Update(float dt);
	virtual void Draw(Shader* shader = nullptr, Surface* Surface = nullptr);


	virtual void handleBeginContact(b2Contact* contact) {}
	virtual void handleEndContact(b2Contact* contact) {}
	virtual void handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold) {}
	virtual void handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {}

	virtual void Debug();
	void ResizePhys();
	unsigned int GetID() { return Id; }

	virtual b2Body* GetBody() { return m_body; }
	virtual b2Fixture* GetFixture() { return m_contactFixture; }
protected:
	bool enabled, enabled_physics, visible;
public: //make getters later...
	//position, rotation_angle, rotation_axis, size, scaling
	glm::vec3 position, size, orientation_axis, rotation_axis;
	float scale, orientation, rotation;
	//texture

	//body, mainFixture
	b2Body* m_body;
	b2Fixture* m_contactFixture;
	b2Vec2 linearVelocity;
	float angularVelocity;
	//other
	std::string name, alias;
	//rendering
	glm::vec4 color;
public:
	void AddPhysics(b2BodyDef*, b2FixtureDef*);
	Texture* m_sprite;
	Surface* m_surface;
	static Surface* g_GameObjectSurface;
	static Shader* g_gameObjectShader;
	bool deletionFlag;
protected:
	b2Shape::Type shapeType;
	unsigned int Id;
private:
	static unsigned int Count;
};

#endif // !GAMEOBJECT_H