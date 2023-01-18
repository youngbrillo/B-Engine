#pragma once
#ifndef PARALLAXLAYER_H
#define PARALLAXLAYER_H

#include "Sprite.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
class ParallaxLayer
{
public:
	ParallaxLayer(Texture* texture, int order, float speed, std::string name = "Parallax Layer", bool _active = true);
	~ParallaxLayer();
	
	Sprite* layer;
	bool visible, active;
	int ordinal;
	float transitionSpeed;
	glm::vec2 origin, offset;
		
	void Debug();

	std::string title;

};
struct ParallaxManagerDefinition
{

	Transform _transform = Transform();
	glm::ivec2 _direction = glm::ivec2(0);

	std::vector<ParallaxLayer*> layers;
	float baseSpeed = 1.0f;
	bool animatedByDefault = false;
};
class ParallaxManager
{
public:
	ParallaxManager(ParallaxManagerDefinition* def);
	~ParallaxManager();

	void Update(float dt);
	void Draw(Shader* shader, Surface* mesh);

	void Debug(const char* name = "Parallax Layer Manager");

public:
	bool visible, animated;
	glm::ivec2 direction;
	Transform transform;
	float baseSpeed;
	std::vector<ParallaxLayer*> layers;

public:
	float animationSpeed, animationProgress, animationEnd;

};



#endif // !PARALLAXLAYER_H
