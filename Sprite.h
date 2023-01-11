#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include "SpriteMap.h"
#include "Shader.h"
#include <glm/glm.hpp>


class Sprite
{
public:
	Sprite(Texture* texture, const char* jsonFile, int defaultIndex = 0);
	Sprite(Texture* texture, const int columns, const int rows, int defaultIndex = 0);

	~Sprite();

	void Update(float dt);
	void Draw(Shader* shader = nullptr);
	void Debug();

public:
	int index, animStartIndex, animEndIndex;
	bool animate, visible;
	glm::vec4 color;
//private:
	SpriteMap* map;
	SpriteSettings settings;
	float frameProgress, frameLength;

};

#endif // !SPRITE_H

