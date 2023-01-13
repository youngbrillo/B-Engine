#pragma once
#ifndef SCROLLINGSPRITE_H
#define SCROLLINGSPRITE_H
#include "Texture.h"
#include "Surface.h"
#include "Shader.h"
#include <vector>
#include "glm/glm.hpp"
#include "SpriteMap.h"
#include "ParallaxLayer.h"
#include <iostream>

struct ScrollingSpriteDefinition
{
	int layerCount = 0;
	std::vector<Texture*> layers;
	float baseScrollSpeed = 10.0f;
	float layerScrollModifier = 0.3f;
	//transform

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 pivot = glm::vec3(0.0f);
	glm::vec3 size = glm::vec3(1.0f);
	float scale = 1.0f;
	glm::vec4 mainColor = glm::vec4(1.0f);

	float speedMultiplier = 0.01f;
	std::string name = "ParallaxLayer";
};

class ScrollingSprite
{
public:
	ScrollingSprite(const ScrollingSpriteDefinition* def);
	~ScrollingSprite();

	void Update(float deltaTime);
	void Draw(Shader* shader);

	void Debug();
protected:
	std::vector<ParallaxLayer*> layers;
public:
	glm::vec3 position, pivot, size;
	glm::vec4 mainColor;
	float scale, zOffset, speedMultiplier;
	bool visible;


	glm::ivec2 direction;
private:
	void init(const ScrollingSpriteDefinition& def);

	std::string name;
};

#endif // !SCROLLINGSPRITE_H