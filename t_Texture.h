#pragma once
#ifndef T_TEXTURE_H
#define T_TEXTURE_H

#include <string>
class t_Texture
{
public:
	t_Texture();
	t_Texture(const char* path, bool alpha);
	t_Texture(const char* path, bool alphaChannel, unsigned int minfilter, unsigned int maxfilter);
	~t_Texture();
	void GenerateTexture(const char* path, bool alphaChannel);

	void Bind();
	void Unbind();
	bool valid() { return generated; }
	unsigned int GetId() { return id; }

public:
	void Debug();
	void RenderDebug() { Debug(); }
	unsigned int Wrap_S, Wrap_T, Filter_Min, Filter_Max;
	int imageWidth, imageHeight;

	//misc image poperties
	int numberofChannels, expectedChannels;

private:
	unsigned int id;
	std::string name;
	int index_s, index_t, index_min, index_max;
	bool generated;
};


#endif // !T_TEXTURE_H