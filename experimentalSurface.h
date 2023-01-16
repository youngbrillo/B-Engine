#pragma once
#include "Surface.h"
#include <vector>

class experimentalSurface : public Surface
{
public:
	experimentalSurface();
	~experimentalSurface();// { Surface::~Surface(); };

	virtual void Generate(float min = 0.0f, float max = 1.0f);
	virtual void Generate(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef);

	virtual void Generate(int rows, int columns, int textureWidth, int textureHeight);

	virtual void Bind();
	virtual void UpdateVerticies(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef);


public:
	float* m_verticies;
	unsigned int mVertexDataBuffer, *mIndexBuffers;
	std::vector<unsigned int> mClips;
};





class experimentalTexture
{
public:
	//experimentalTexture() : id(0), Wrap_S(0), Wrap_T(0), Filter_Min(0), Filter_Max(0), width(-1), height(-1), channelCount(-1) {}
	experimentalTexture(const char* path, bool alpha, int rowCount, int columnCount);
	~experimentalTexture();

	void Bind();
	void Bind(int index);
	void Unbind();
	unsigned int GetId() { return id; }
private:
	unsigned int id, Wrap_S, Wrap_T, Filter_Min, Filter_Max;
	void GenerateTexture(const char* path, bool alphaChannel, unsigned int& textureId, int& _width, int& _height, int& numOfChannels, int expectedChannels = 0);
public:
	void RenderDebug();
	static Surface* defaultSurface;
	int width, height, channelCount, rows, columns;
    unsigned int mIboId;

	std::vector<glm::vec2> clips;
	glm::ivec2 clipSize;
};

struct eFrame{
	unsigned int textureId;
	glm::ivec2 size; //size of the sprite in the sprite sheet
	glm::ivec2 offset; //offset from baseline to left/top of sprite
	unsigned int advance;//horizontal offset to next sprite

};