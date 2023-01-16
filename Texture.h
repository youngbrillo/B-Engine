#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
	Texture();// : id(0), Wrap_S(0), Wrap_T(0), Filter_Min(0), Filter_Max(0), width(-1), height(-1){}
	Texture(const char* path, bool alpha = true);
	Texture(const char* path, bool alphaChannel, unsigned int minfilter, unsigned int maxfilter);
	~Texture();

	void Bind();
	void Unbind();
	unsigned int GetId() { return id; }
	void generateTexture(const char* path, bool alphaChannel);

public:
	void RenderDebug();
	int width, height,numberofChannels, expectedChannels;

private:
	unsigned int id, Wrap_S, Wrap_T, Filter_Min, Filter_Max;
	std::string name;
	int index_s, index_t, index_min, index_max;
	bool generated;

	friend class ResourceManager;
};

void GenerateTexture(const char* path, bool alphaChannel, unsigned int& textureId, int& _width, int& _height, int& numOfChannels, int expectedChannels = 0);

#endif // !SPRITE_H