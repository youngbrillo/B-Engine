#pragma once
#ifndef SURFACE_H
#define SURFACE_H

#include <glm/glm.hpp>
class Surface
{
public:
	Surface();
	~Surface();// { printf("Deleting Surface: %i", VAO);  glDeleteVertexArrays(1, &VAO); glDeleteBuffers(1, &VBO); };

	virtual void Generate(float min = 0.0f, float max =1.0f) = 0;
	virtual void Generate(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef) = 0;
	virtual void Bind() = 0;
public:
	static void unBind();// { glBindVertexArray(0); }
	unsigned int getVAO() { return VAO; }
protected:
	unsigned int VAO, VBO;
};


class quadSurface : public Surface
{
public:
	quadSurface() : Surface() {};
	~quadSurface() { Surface::~Surface(); };

	virtual void Generate(float min = 0.0f, float max = 1.0f);
	virtual void Generate(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef);
	virtual void Bind();
};

class cubeSurface : public Surface
{
public:
	cubeSurface() : Surface() {};
	~cubeSurface() { Surface::~Surface(); };
	virtual void Generate(float min = 0.0f, float max = 1.0f);
	virtual void Generate(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef) { printf("Not implemented\n"); }
	virtual void Bind();
};



#endif // Surface
