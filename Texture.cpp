#include "Texture.h"
#include "imgui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <glad/glad.h>

Surface* Texture::defaultSurface = nullptr;



Texture::Texture(const char* path, bool alpha)
	: id(0)
	, Wrap_S(GL_REPEAT)
	, Wrap_T(GL_REPEAT)
	, Filter_Min(GL_LINEAR)
	, Filter_Max(GL_LINEAR)
{
	GenerateTexture(path, alpha, id);
}
Texture::~Texture()
{
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


void Texture::RenderDebug()
{


	if (ImGui::TreeNode("Texture 2D settings")) {


		static unsigned int textureWrapping[4] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };
		const char* textureWrapNames[4] = { "GL_REPEAT", "GL_MIRRORED_REPEAT", "GL_CLAMP_TO_EDGE", "GL_CLAMP_TO_BORDER" };

		static int tw_index_s = 0;
		static int tw_index_t = 0;

		static unsigned int textureFilters[5] = { GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR };
		const char* textureFilterNames[5] = { "GL_NEAREST", "GL_LINEAR", "GL_NEAREST_MIPMAP_NEAREST", "GL_LINEAR_MIPMAP_NEAREST", "GL_LINEAR_MIPMAP_LINEAR" };

		static int tf_index_min = 0;
		static int tf_index_max = 0;

		if (ImGui::SliderInt("Wrap S", &tw_index_s, 0, 3, textureWrapNames[tw_index_s]))
		{
			Wrap_S = textureWrapping[tw_index_s];
		}
		if (ImGui::SliderInt("Wrap T", &tw_index_t, 0, 3, textureWrapNames[tw_index_t]))
		{
			Wrap_T = textureWrapping[tw_index_t];

		}
		if (ImGui::SliderInt("Filter min", &tf_index_min, 0, 4, textureFilterNames[tf_index_min]))
		{
			Filter_Min = textureFilters[tf_index_min];
		}
		if (ImGui::SliderInt("Filter max", &tf_index_max, 0, 4, textureFilterNames[tf_index_max]))
		{
			Filter_Max = textureFilters[tf_index_max];
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

	if (_width > -1)
		_width = w;
	if (_height > -1)
		_height = h;
	if (numOfChannels > -1)
		numOfChannels = nbrofChannels;
}





/*
* 
void t_Texture::RenderDebug()
{

	if (ImGui::TreeNode("Texture 2D settings")) {


		static unsigned int textureWrapping[4] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };
		const char* textureWrapNames[4] = { "GL_REPEAT", "GL_MIRRORED_REPEAT", "GL_CLAMP_TO_EDGE", "GL_CLAMP_TO_BORDER" };

		static int tw_index_s = 0;
		static int tw_index_t = 0;

		static unsigned int textureFilters[5] = { GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR };
		const char* textureFilterNames[5] = { "GL_NEAREST", "GL_LINEAR", "GL_NEAREST_MIPMAP_NEAREST", "GL_LINEAR_MIPMAP_NEAREST", "GL_LINEAR_MIPMAP_LINEAR" };

		static int tf_index_min = 0;
		static int tf_index_max = 0;

		if (ImGui::SliderInt("Wrap S", &tw_index_s, 0, 3, textureWrapNames[tw_index_s]))
		{
			Wrap_S = textureWrapping[tw_index_s];
		}
		if (ImGui::SliderInt("Wrap T", &tw_index_t, 0, 3, textureWrapNames[tw_index_t]))
		{
			Wrap_T = textureWrapping[tw_index_t];

		}
		if (ImGui::SliderInt("Filter min", &tf_index_min, 0, 4, textureFilterNames[tf_index_min]))
		{
			Filter_Min = textureFilters[tf_index_min];
		}
		if (ImGui::SliderInt("Filter max", &tf_index_max, 0, 4, textureFilterNames[tf_index_max]))
		{
			Filter_Max = textureFilters[tf_index_max];
		}


		ImGui::TreePop();
	}
}
*/
