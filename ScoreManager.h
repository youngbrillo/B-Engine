#pragma once
#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H
#include "ObjectListener.h"

namespace SpaceBuster {
	enum comboTier {
		ctLevelOne		= 1,
		ctLevelTwo		= 2,
		ctLevelThree	= 3,
		ctLevelFour		= 4,
		ctLevelFive		= 5
	};
	class ScoreManager //: public ObjectListener
	{
	public:
		ScoreManager(float comboLength = 3.0f, int levelGoal = 10);
		~ScoreManager();

		void onScore(int baseIncrement = 1);
		void IncrementScore(int baseIncrement = 1);
		void onWaveClear();
		void Update(float dt);
		void Debug(const char* name = "Score Manager");
		void Draw(int baseXoffset = 10, int baseYoffset = 10);


		//virtual void ObjectCreated(GameObject* go) override;
		//virtual void ObjectDeleted(GameObject* go) override;

		const int getScore() { return score; }
		const int setScore(int mod) { score += mod; return score; }
	private:

		void advanceLevels(comboTier& tier);
		int score, scoreMultiplier, nextLevelProgress, nextLevelGoal;
		float comboProgress, comboMaxLength, comboMultiplier;
		bool comboValid;
		comboTier comboLevel;
	private: 
		int debugIndex = 0;
	};
};
#endif // !SCOREMANAGER_H