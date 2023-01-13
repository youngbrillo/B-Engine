#pragma once
#ifndef SPRITEMAP_H
#define SPRITEMAP_H

#include "Texture.h"
//#include "Shader.h"
#include "Surface.h"
#include <glm/glm.hpp>

#include "experimentalSurface.h"

class SpriteSettings
{
public:
	SpriteSettings() {}
	~SpriteSettings() {}

	int frameCount = 0;
	int img_size_x = 0;
	int img_size_y = 0;
	glm::u32vec2* framesArray = nullptr;
	int framesArraySize = 0;
	int rowCount = 0;
	int columnCount = 0;
};


class SpriteMap
{
public:
	SpriteMap(Texture* texture, const char* jsonFile, SpriteSettings& settings);
	SpriteMap(Texture* texture, const int columns, const int rows);
	~SpriteMap();

	//void Draw(Shader* shader);

	void GetFrame(const int x, const int y);

	void GetFrame(const int index);

	void Debug(const char* name = nullptr);

	void Draw();
	void Draw(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d);
	void configFromJson(const char* jsonFile, SpriteSettings& settings);

	glm::vec2 getElementSize() const { return elementSize; }
	glm::i32vec2 getrowcolumnDimensions() const { return rowcolumnDimensions; }
protected:
	void configureTextureCoords();
private:
	Surface* m_surface;
	experimentalSurface* k_surface;
	//a = top right corner, b = bottom right, c = bottom left, d = top left
	glm::vec2 a, b, c, d;
	//in a tightly packed sprite sheet, the element size is the element of each sprite
	glm::vec2 elementSize;
	glm::i32vec2 rowcolumnDimensions;
	Texture* TextureReference;


	friend class Sprite;
public:
	void configureTextureCoords(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d);
	bool RenderLazy;
};

#endif // !SPRITEMAP_H

