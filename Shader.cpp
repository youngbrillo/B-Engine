#include "Shader.h"
#include <glad/glad.h>


#include <sstream>
#include <fstream>

Shader::Shader(const char* vssSource, const char* fssSource, bool isFile)
{
	if (isFile)
	{
		std::string vss = readShaderFile(vssSource);
		std::string fss = readShaderFile(fssSource);
		//printf("output shader is:\n%s\n----------------------------------------\n", vss.c_str());

		compileProgramId(vss.c_str(), fss.c_str(), this->id);
	}
	else
	{
		//printf("Vertex Shader:\n%s\n--------------------------------------\n", vssSource);
		compileProgramId(vssSource, fssSource, this->id);
	}
	//printf("Creating Shader (%d) from:\t'%s' & '%s'\n", id, vssSource, fssSource);
}

Shader::~Shader()
{
	//printf("Deleting Shader (%d)...", id);
	glDeleteProgram(this->id);
	id = 0;
	//printf("%d\n", id);
}

Shader& Shader::Use()
{
	glUseProgram(id);
	return *this;
}

Shader& Shader::setInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
	return *this;
}

Shader& Shader::setFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(id, name), value);
	return *this;
}

Shader& Shader::SetVector2f(const char* name, float x, float y)
{
	glUniform2f(glGetUniformLocation(id, name), x, y);
	return *this;
}

Shader& Shader::SetVector2f(const char* name, const glm::vec2& value)
{
	// // O: insert return statement here
	glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
	return *this;
}

Shader& Shader::SetVector3f(const char* name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(id, name), x, y, z);
	return *this;
}

Shader& Shader::SetVector3f(const char* name, const glm::vec3& value)
{
	glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
	return *this;
}

Shader& Shader::SetVector4f(const char* name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
	return *this;
}

Shader& Shader::SetVector4f(const char* name, const glm::vec4& value)
{
	glUniform4f(glGetUniformLocation(id, name), value.x, value.y, value.z, value.w);

	return *this;
}

Shader& Shader::SetMatrix4(const char* name, const glm::mat4& matrix)
{
	// // O: insert return statement here
	glUniformMatrix4fv(glGetUniformLocation(id, name), 1, false, glm::value_ptr(matrix));
	return *this;
}


//utility functions
void compileProgramId(const char* vertShaderSource, const char* fragShaderSource, unsigned int& progId)
{
	unsigned int vertShader, fragShader;
	compileShader(vertShaderSource, vertShader, GL_VERTEX_SHADER);
	compileShader(fragShaderSource, fragShader, GL_FRAGMENT_SHADER);

	progId = glCreateProgram();
	glAttachShader(progId, vertShader);
	glAttachShader(progId, fragShader);
	glLinkProgram(progId);
	gl_errorCheck(progId, "LINK", true);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

void compileShader(const char* shaderSource, unsigned int& shaderId, unsigned int shader_type)
{
	shaderId = glCreateShader(shader_type);
	glShaderSource(shaderId, 1, &shaderSource, NULL);
	glCompileShader(shaderId);
	gl_errorCheck(shaderId, shader_type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : "GL_FRAGMENShader", false);
}

void gl_errorCheck(unsigned int& obj, const char* type, bool isProgram)
{
	int success;
	char infoLog[512];
	const char* _type = isProgram ? "_Program_" : "_Shader_";
	if (!isProgram)
	{
		glGetShaderiv(obj, GL_COMPILE_STATUS, &success);
	}
	else
	{
		glGetProgramiv(obj, GL_LINK_STATUS, &success);
	}
	if (!success)
	{
		if (!isProgram)
			glGetShaderInfoLog(obj, 512, NULL, infoLog);
		else
			glGetProgramInfoLog(obj, 512, NULL, infoLog);

		printf("ERROR::%s::%s::COMPILATION_FAILED\n\t%s\n", _type, type, infoLog);
		printf("_________________________________________________________________________\n");
	}
	else {
		//printf("Success::%s::%s::COMPIlation acchieved!!\n", _type, type);
	}
}

std::string readShaderFile(const char* path)
{
	std::string shaderCode;

	try
	{
		std::ifstream shaderFile(path);
		std::stringstream shaderStream;

		//read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		//close file handlers
		shaderFile.close();

		//convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::exception e)
	{
		printf("Shader::CPP::ERROR::readShaderFile::\t[unknown exception] failed to read shader file: <%s>\n\t%s\n", path, e.what());
	}

	return shaderCode.c_str();
}
