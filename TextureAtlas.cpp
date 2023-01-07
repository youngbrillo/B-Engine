#include "TextureAtlas.h"
#include <glad/glad.h>

#include "stb_image.h"

TextureAtlas::TextureAtlas()
	: id(0)
	, Wrap_S(GL_REPEAT)
	, Wrap_T(GL_REPEAT)
	, Filter_Min(GL_LINEAR)
	, Filter_Max(GL_LINEAR)
	, spriteWidth(0)
	, spriteHeight(0)
	, indexX(0)
	, indexY(0)
	, expectedChannels(0)
	, numberOfChannels(0)
	, textureMode(true)
	, ref(nullptr)
	, rowSize(1), columnSize(1)
{
}

TextureAtlas::TextureAtlas(const char* path, int numberOfRows, int numberOfColumns, bool alpha)
	: id(0)
	, Wrap_S(GL_REPEAT)
	, Wrap_T(GL_REPEAT)
	, Filter_Min(GL_LINEAR)
	, Filter_Max(GL_LINEAR)
	, spriteWidth(0)
	, spriteHeight(0)
	, indexX(0)
	, indexY(0)
	, expectedChannels(0)
	, numberOfChannels(0)
	, textureMode(true)
	, ref(nullptr)
	, rowSize(numberOfRows), columnSize(numberOfColumns)
{
	this->Load(path, alpha);
}

TextureAtlas::TextureAtlas(Texture* mainTex, const glm::vec2& min, const glm::vec2& max)
	: ref(mainTex)
	, textureMode(false)
	, rowSize(1), columnSize(1)

{
	m_TexCoords[0] = { min.x, min.y }; //bottom left 
	m_TexCoords[1] = { max.x, min.y }; //bottom right 
	m_TexCoords[2] = { max.x, max.y }; //top right
	m_TexCoords[3] = { min.x, max.y }; //top left
}

TextureAtlas::~TextureAtlas()
{
}

void TextureAtlas::Load(const char* path, bool hasAlphaChannel)
{

	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int w, h, nbrofChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &w, &h, &nbrofChannels, expectedChannels);

	GLint internalFormat = hasAlphaChannel ? GL_RGBA : GL_RGBA;

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

	spriteWidth = w;
	spriteHeight = h;
	numberOfChannels = nbrofChannels;
}

void TextureAtlas::GenerateTexture()
{
}

void TextureAtlas::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);
}

void TextureAtlas::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureAtlas::DrawSprite(float posX, float posY, int frameIndex)
{
	const float verts[] = {
		 posX, posY,
		 posX + spriteWidth, posY,
		 posX + spriteWidth, posY + spriteHeight,
		 posX, posY + spriteHeight
	};

	int texWidth = spriteWidth / rowSize;
	int texHeight = spriteHeight / columnSize;

	const float tw = float(spriteWidth) / texWidth;
	const float th = float(spriteHeight) / texHeight;
	const int numPerRow = texWidth / spriteWidth;
	const float tx = (frameIndex % numPerRow) * tw;
	const float ty = (frameIndex / numPerRow + 1) * th;
	const float texVerts[] = {
		tx, ty,
		tx + tw, ty,
		tx + tw, ty + th,
		tx, ty + th
	};

	// ... Bind the texture, enable the proper arrays
	//this->Bind();
	//
	//glVertexPointer(2, GL_FLOAT, verts);
	//glTexCoordPointer(2, GL_FLOAT, texVerts);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

#include "imgui.h"

void TextureAtlas::RenderDebug()
{
	if (ImGui::TreeNode("- Atlas -")) {

		ImGui::SliderInt("Atlas width", &spriteWidth, 0, 0);
		ImGui::SliderInt("Atlas height", &spriteHeight, 0, 0);


		ImGui::TreePop();
	}


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
