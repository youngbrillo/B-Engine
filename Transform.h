#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Transform
{
public:
	Transform();
	~Transform();
	
	glm::mat4& Model();
	void UpdateMatrix();

	void Debug(const char* owner);


public:
	glm::vec3 position, size, pivot, orientation;
	glm::mat4 m_model;
	float rotation, scale;
};

#endif// !TRANSFORM_H