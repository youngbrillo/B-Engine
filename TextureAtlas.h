#pragma once
#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H


#include "Surface.h"
#include "Shader.h"

#include "Texture.h"
#include <glm/common.hpp>

class TextureAtlas
{
public:
	TextureAtlas();
	TextureAtlas(const char* path, int numberOfRows, int numberOfColumns, bool alpha = true);
	TextureAtlas(Texture* mainTex, const glm::vec2& min, const glm::vec2& max);
	~TextureAtlas();

	void Load(const char* path, bool hasAlphaChannel);
	void GenerateTexture();
	void Bind();
	void Unbind();
	unsigned int GetId() { return id; }
private:
	unsigned int id, Wrap_S, Wrap_T, Filter_Min, Filter_Max;
	int spriteWidth, spriteHeight, indexX, indexY, expectedChannels, numberOfChannels;
	bool textureMode; //true is textureMode, false is Atlas mode

private:
	//atlas mode variables
	Texture* ref;
	glm::vec2 m_TexCoords[4];

public:
	Texture* createFromCoords;
	int rowSize, columnSize;

	void DrawSprite(float posX, float posY, int frameIndex);

public:
	void RenderDebug();
};

#endif // !TEXTUREATLAS_H
