#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "Surface.h"
#include "Shader.h"

class Texture
{
public:
	Texture() : id(0), Wrap_S(0), Wrap_T(0), Filter_Min(0), Filter_Max(0), width(-1), height(-1){}
	Texture(const char* path, bool alpha = true);
	~Texture();

	void Bind();
	void Unbind();
	unsigned int GetId() { return id; }
private:
	unsigned int id, Wrap_S, Wrap_T, Filter_Min, Filter_Max;

	friend class ResourceManager;
public:
	void RenderDebug();
	static Surface* defaultSurface;
	int width, height;
};

static int DEFAULTPARAM_INT = -1;
void GenerateTexture(const char* path, bool alphaChannel, unsigned int& textureId, int& _width = DEFAULTPARAM_INT, int& _height = DEFAULTPARAM_INT, int& numOfChannels = DEFAULTPARAM_INT, int expectedChannels = 0);

#endif // !SPRITE_H