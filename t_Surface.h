#pragma once

#ifndef T_SURFACE_H
#define T_SURFACE_H

#include <glm/glm.hpp>


class t_Surface
{
public:
	t_Surface(bool generateOk);
	~t_Surface();
	virtual void Bind();
	static void unBind();
	unsigned int getVAO() { return vertex_array_object; }

	virtual void Generate(float min = 0.0f, float max = 1.0f);
	virtual void Generate(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef);
	virtual void Generate(int index);
public:
	void Debug(const char* name);
protected:
	unsigned int vertex_array_object, vertex_buffer_object;
	float vertexArray[20];
	int vertexArraySize;
	bool enabled;

	unsigned int mode;
	int modeIndex;
};


#endif // Surface
