#pragma once
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "GameObject.h"
#include <vector>
#include "Shader.h"
#include "Surface.h"
#include <Box2D/Box2D.h>

class ObjectFactory
{
public:
	//This class handles all deletes for the GameObject class and the initalization
	static void InsertObject(GameObject* newObject);
	static std::vector<GameObject*>::iterator DestroyObject(GameObject* existingObject);

	static void UpdateObjects(float dt);
	static void FixedUpdateObjects(float dt);


	static void handleBeginContact(b2Contact* contact);
	static void handleEndContact(b2Contact* contact);
	static void handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	static void handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	static void DrawObjects(Shader* globalSpriteShader = NULL, Surface* globalSpriteSurface = NULL);
	static void Debug();


	static void cleanup();
private:
	ObjectFactory();
	static std::vector<GameObject*> ALLGAMEOBJECTS;
};

#endif // !OBJECTFACTORY_H