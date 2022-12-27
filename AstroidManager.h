#pragma once
#ifndef ASTROIDMANAGER_H
#define ASTROIDMANAGER_H


#include "AstroidObject.h"
#include <vector>
namespace SpaceBuster {

	class AstroidFactory {
	public:
		AstroidFactory(const unsigned int limit = 50, const b2Vec2& lowerBound = b2Vec2(20, 20), const b2Vec2& upperBound = b2Vec2(20, 20));
			
		~AstroidFactory();// { groundRef = nullptr; Astroids.clear(); }

		void createAstroid(const b2Vec2& s, const b2Vec2& lv, const float& av, b2Vec2 size = b2Vec2(1.0f, 1.0f), bool overrideCountRestriction = false);
		void Update(float dt);
		void autoGenerate(float dt);
		void Debug();
		void handleDelete(AstroidObject* i);
		void handleFragment(AstroidState& state,AstroidObject* i);
	private:
		std::vector<AstroidState> listToCreate;
		int max, count;
		float timeLeftUntilNextSpawn, SpawnTimeTarget, spawnAngle, maxAstroidSize, baseChance;

		b2Vec2 spawnUpperBound, spawnLowerBound,
			lastDeletedPos, lastDeletedSpeed;
	public:
		bool autoGenerateEnabled, lastFrameDelete;
		b2Body* groundRef;
		int amountDeleted;
		GameObjectDefinition def_template;


		//static AstroidFactory* g_Instance;
	};



}

#endif // !ASTROIDMANAGER_H
