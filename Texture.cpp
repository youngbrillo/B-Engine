#include "Texture.h"
#include "imgui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <glad/glad.h>


const unsigned int _textureWrapping[4] =
{ GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };
const char* _textureWrapNames[4] =
{ "GL_REPEAT", "GL_MIRRORED_REPEAT", "GL_CLAMP_TO_EDGE", "GL_CLAMP_TO_BORDER" };

const unsigned int _textureFilters[6] =
{ GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR };
const char* _textureFilterNames[6] =
{ "GL_NEAREST", "GL_LINEAR", "GL_NEAREST_MIPMAP_NEAREST", "GL_NEAREST_MIPMAP_LINEAR", "GL_LINEAR_MIPMAP_NEAREST", "GL_LINEAR_MIPMAP_LINEAR" };



Texture::Texture()
	: id(0)
	, Wrap_S(0)
	, Wrap_T(0)
	, Filter_Min(0)
	, Filter_Max(0)
	, width(-1)
	, height(-1)
	, name("- EMPTY TEXTURE -")
	, index_s(0), index_t(0), index_min(0), index_max(0)
	, numberofChannels(-1)
	, expectedChannels(0)
	, generated(false)
{
}
Texture::Texture(const char* path, bool alpha)
	: id(0)
	, Wrap_S(GL_REPEAT)
	, Wrap_T(GL_REPEAT)
	, Filter_Min(GL_LINEAR)
	, Filter_Max(GL_LINEAR)
	, width(-1), height(-1)
	, index_s(0), index_t(0), index_min(0), index_max(0)
	, numberofChannels(-1)
	, expectedChannels(0)
	, generated(false)
{

	name = "Texture: ";
	name += path;

	for (int i = 0; i < sizeof(_textureFilters) / sizeof(_textureFilters[0]); i++)
	{
		if (_textureFilters[i] == Filter_Min)
			index_min = i;
		if (_textureFilters[i] == Filter_Max)
			index_max = i;
	}
	this->generateTexture(path, alpha);
	//GenerateTexture(path, alpha, id, width, height, numberofChannels, expectedChannels);

}
Texture::Texture(const char* path, bool alphaChannel, unsigned int minfilter, unsigned int maxfilter)
	: id(0)
	, Wrap_S(GL_REPEAT)
	, Wrap_T(GL_REPEAT)
	, Filter_Min(minfilter)
	, Filter_Max(maxfilter)
	, width(-1), height(-1)
	, index_s(0), index_t(0), index_min(0), index_max(0)
	, numberofChannels(-1)
	, expectedChannels(0)
	, generated(false)
{
	name = "Texture: "; name += path;
	for (int i = 0; i < sizeof(_textureFilters) / sizeof(_textureFilters[0]); i++)
	{
		if (_textureFilters[i] == Filter_Min)
			index_min = i;
		if (_textureFilters[i] == Filter_Max)
			index_max = i;
	}
	this->generateTexture(path, alphaChannel);
}
Texture::~Texture()
{
	//glDeleteTextures(1, &id);
}

void Texture::Bind()
{
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::generateTexture(const char* path, bool alphaChannel)
{
	//GLint internalFormat = alphaChannel ? GL_RGBA : GL_RGB;
	GLint internalFormat = alphaChannel ? GL_RGBA : GL_RGBA;

	//lock new texture to new texture context
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &numberofChannels, expectedChannels);
	if (!data)
	{
		printf("Failed to load texture: '%s'\n", path);
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}


	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	generated = true;

	//free texture context
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

}


void Texture::RenderDebug()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		if (ImGui::SliderInt("Wrap S", &index_s, 0, 3, _textureWrapNames[index_s]))
		{
			Wrap_S = _textureWrapping[index_s];
		}
		if (ImGui::SliderInt("Wrap T", &index_t, 0, 3, _textureWrapNames[index_t]))
		{
			Wrap_T = _textureWrapping[index_t];

		}
		if (ImGui::SliderInt("Filter min", &index_min, 0, 5, _textureFilterNames[index_min]))
		{
			Filter_Min = _textureFilters[index_min];
		}
		if (ImGui::SliderInt("Filter max", &index_max, 0, 5, _textureFilterNames[index_max]))
		{
			Filter_Max = _textureFilters[index_max];
		}

		if (ImGui::Button("print info"))
		{
			printf("Image:\t'%s'\n\tDimensions:\t%d x %d\n\tChannels:\t%d\n", name.c_str(), width, height, numberofChannels);
		}


		ImGui::TreePop();
	}
}

void GenerateTexture(const char* path, bool alphaChannel, unsigned int& textureId, int& _width, int& _height, int& numOfChannels, int expectedChannels)
{

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int w, h, nbrofChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &w, &h, &nbrofChannels, expectedChannels);

	GLint internalFormat = alphaChannel ? GL_RGBA : GL_RGBA;

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, internalFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		printf("Failed to load texture: '%s'\n", path);
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	_width = w;
	_height = h;
	numOfChannels = nbrofChannels;
}
