#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Shader
{
public:
	Shader() : id(0) {}
	Shader(const char* vssSource, const char* fssSource, bool isFile = false);

	~Shader();

	Shader& Use();
	Shader& setInt(const char* name, int value);
	Shader& setFloat(const char* name, float value);

	Shader& SetVector2f(const char* name, float x, float y);
	Shader& SetVector2f(const char* name, const glm::vec2& value);
	Shader& SetVector3f(const char* name, float x, float y, float z);
	Shader& SetVector3f(const char* name, const glm::vec3& value);
	Shader& SetVector4f(const char* name, float x, float y, float z, float w);
	Shader& SetVector4f(const char* name, const glm::vec4& value);
	Shader& SetMatrix4(const char* name, const glm::mat4& matrix);

	unsigned int GetId() { return id; }
private:
	unsigned int id;
};


void compileProgramId(const char* vertShaderSource, const char* fragShaderSource, unsigned int& progId);
void compileShader(const char* shaderSource, unsigned int& shaderId, unsigned int shader_type);
void gl_errorCheck(unsigned int& obj, const char* type, bool isProgram);
std::string readShaderFile(const char* path);

#endif // !SHADER_H
