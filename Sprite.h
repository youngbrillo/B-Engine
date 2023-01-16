#pragma once
#ifndef SPRITE_H
#define SPRITE_H
#include "Shader.h"
#include "Surface.h"
#include "Transform.h"
#include "Texture.h"

class Sprite
{
public:
	Sprite();
	Sprite(Texture* t, int columns = 1, int rows = 1);
	~Sprite();

	void setTexture(Texture* t) { texture = t; }
	void Update(float dt);
	void Draw(Shader* shader, Surface* mesh);
	void Debug(const char* name = nullptr);

public:
	Transform transform;
	glm::vec4 Color;
	int index, frameStart, frameEnd, maxFrame;
	glm::vec2 spriteSize, dimensions;
private:
	Texture* texture;

public:
	bool animated, visible;
	float frame_time_length, frame_time_remaining;
};

#endif