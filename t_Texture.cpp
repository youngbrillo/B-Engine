#include "t_Texture.h"

#include "imgui.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

const unsigned int textureWrapping[4] = 
{ GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };
const char* textureWrapNames[4] = 
{ "GL_REPEAT", "GL_MIRRORED_REPEAT", "GL_CLAMP_TO_EDGE", "GL_CLAMP_TO_BORDER" };

const unsigned int textureFilters[6] = 
{ GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR };
const char* textureFilterNames[6] = 
{ "GL_NEAREST", "GL_LINEAR", "GL_NEAREST_MIPMAP_NEAREST", "GL_NEAREST_MIPMAP_LINEAR", "GL_LINEAR_MIPMAP_NEAREST", "GL_LINEAR_MIPMAP_LINEAR" };


t_Texture::t_Texture()
	: id(0)
	, Wrap_S(0)
	, Wrap_T(0)
	, Filter_Min(0)
	, Filter_Max(0)
	, imageWidth(-1)
	, imageHeight(-1)
	, name("- EMPTY TEXTURE -")
	, index_s(0), index_t(0), index_min(0), index_max(0)
	, numberofChannels(-1)
	, expectedChannels(0)
	, generated(false)
{
}

t_Texture::t_Texture(const char* path, bool alpha)
	: id(0)
	, Wrap_S(GL_REPEAT)
	, Wrap_T(GL_REPEAT)
	, Filter_Min(GL_NEAREST)
	, Filter_Max(GL_NEAREST)
	, imageWidth(-1), imageHeight(-1)
	, index_s(0), index_t(0), index_min(0), index_max(0)
	, numberofChannels(-1)
	, expectedChannels(0)
	, generated(false)
{
	name = "Texture: ";
	name += path;

	this->GenerateTexture(path, alpha);
}

t_Texture::t_Texture(const char* path, bool alphaChannel, unsigned int minfilter, unsigned int maxfilter)
	: id(0)
	, Wrap_S(GL_REPEAT)
	, Wrap_T(GL_REPEAT)
	, Filter_Min(minfilter)
	, Filter_Max(maxfilter)
	, imageWidth(-1), imageHeight(-1)
	, index_s(0), index_t(0), index_min(0), index_max(0)
	, numberofChannels(-1)
	, expectedChannels(0)
	, generated(false)
{
	name = "Texture: "; name += path;
	
	//printf("Size: %d/%d = %d\n", sizeof(textureFilters), sizeof(textureFilters[0]), sizeof(textureFilters) / sizeof(textureFilters[0]));
	for (int i = 0; i < sizeof(textureFilters) / sizeof(textureFilters[0]) ; i++)
	{
		if (textureFilters[i] == Filter_Min)
			index_min = i;
		if (textureFilters[i] == Filter_Max)
			index_max = i;
	}

	this->GenerateTexture(path, alphaChannel);
}

t_Texture::~t_Texture()
{
	glDeleteTextures(1, &id);
}

void t_Texture::GenerateTexture(const char* path, bool alphaChannel)
{

	//lock new texture to new texture context
	glGenTextures(1, &id); 
	glBindTexture(GL_TEXTURE_2D, id);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &imageWidth, &imageHeight, &numberofChannels, expectedChannels);
	GLint internalFormat = alphaChannel ? GL_RGBA : GL_RGB;
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageWidth, imageHeight, 0, internalFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		generated = true;
	}
	else {
		printf("Failed to load texture: '%s'\n", path);
	}

	//free texture context
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void t_Texture::Bind()
{
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);
}

void t_Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void t_Texture::Debug()
{

	if (ImGui::TreeNode(name.c_str()))
	{

		if (ImGui::SliderInt("Wrap S", &index_s, 0, 3, textureWrapNames[index_s]))
		{
			Wrap_S = textureWrapping[index_s];
		}
		if (ImGui::SliderInt("Wrap T", &index_t, 0, 3, textureWrapNames[index_t]))
		{
			Wrap_T = textureWrapping[index_t];

		}
		if (ImGui::SliderInt("Filter min", &index_min, 0, 5, textureFilterNames[index_min]))
		{
			Filter_Min = textureFilters[index_min];
		}
		if (ImGui::SliderInt("Filter max", &index_max, 0, 5, textureFilterNames[index_max]))
		{
			Filter_Max = textureFilters[index_max];
		}

		if (ImGui::Button("print info"))
		{
			printf("Image:\t'%s'\n\tDimensions:\t%d x %d\n\tChannels:\t%d\n", name.c_str(), imageWidth, imageHeight, numberofChannels);
		}

		ImGui::TreePop();
	}
}
