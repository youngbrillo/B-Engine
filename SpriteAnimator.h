#pragma once
#include <glm/glm.hpp>
#include "Sprite.h"

struct Animation
{
	Animation() : name("Unnamed Animation"), animBounds(0, 1), doOnce(false) {};
	Animation(std::string name) : name(name), animBounds(0, 1), doOnce(false) {};
	Animation(std::string name,glm::ivec2 b, bool singleUse = false) : name(name), animBounds(b), doOnce(singleUse) {};
	Animation(std::string name, int a, int b, bool singleUse = false) : name(name), animBounds(a, b), doOnce(singleUse) {};
	~Animation() {}

	void setAnimation(Sprite* sprite, Animation* next = nullptr)
	{
		sprite->frameStart = animBounds.x;
		sprite->frameEnd = animBounds.y;
		sprite->index = animBounds.x;
		sprite->animated = true;

	}

	glm::ivec2 animBounds;
	bool doOnce;
	std::string name;

	void Debug(const char* title = nullptr);
};
#include <vector>
#include <map>
#include <string>
class SpriteAnimator
{
public:
	SpriteAnimator(Sprite* sprite);
	SpriteAnimator(Sprite* sprite, const char* filepath);
	~SpriteAnimator();

	void Update(float dt);
public:
	//setters

	void addAnimation(Animation* animation, bool isEntry = false);

	void setAnimation(int index);
	void setAnimation(std::string name);

public:
	//getters
	Animation* getAnimation(int index);
	Animation* getAnimation(std::string name);

public: 
	//helpers
	void Debug(const char* title = "- Sprite Animator -");
private:
	std::map<int, Animation*> animationMap;
	std::vector<Animation*> Animations;

	Animation* currentAnimation, *entryAnimation;
	Sprite* ref;
	bool animationChange, animationRefFileExists;
	std::string animationRefFile;
private:
	void LoadAnimationsFromFile_json(const char* filepath);
	void clearAnimations();
};

