#include "TextRenderer.h"
#include <iostream>
#include <glad/glad.h>
#include "Game.h"

Shader* TextRenderer::fontShader = nullptr;

TextRenderer::TextRenderer(const char* filepath)
	: transform()
	, fontColor(1.0f)
{
	LoadFont(filepath);
	if (!fontShader)
	{
		printf("Initializing static font shader...\n");
		fontShader = new Shader("textShader.vertex", "textShader.frag", true);
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Game::AppCam->Width), 0.0f, static_cast<float>(Game::AppCam->Height));
		fontShader->Use().SetMatrix4("projection", projection);
		printf("Scree Dimensions: (%d, %d)\n", Game::AppCam->Width, Game::AppCam->Height);
	}

	//setup VAO/VBO for texutre quads
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::LoadFont(const char* filepath)
{
	if(Characters.size() > 0) Characters.clear();

	FT_Library ft;
	FT_Face face;
	unsigned int errors = 0;
	if (FT_Init_FreeType(&ft)) { printf("ERROR::FREETYPE: Could not init FreeType Library\n"); errors++; }
	if (FT_New_Face(ft, filepath, 0, &face)) { printf("ERROR::FREETYPE: Failed to load font from: '%s'\n", filepath); errors++; }

	if (errors > 0) { printf("error count %d", errors); return; }

	FT_Set_Pixel_Sizes(face, 0, 36);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			printf("Error::FreeType::\tFailed to load glyph %c\n", c);
			continue;
		}
		//GENERATE TEXTURE
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		//SET TEXTURE OPTIONS
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//STORE THE CHARACTER FOR LATER USE...
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
	
		Characters.insert(std::pair<char, Character>(c, character));

	}
	glBindTexture(GL_TEXTURE_2D, 0);


	FT_Done_Face(face);
	FT_Done_FreeType(ft);


}

void TextRenderer::DrawText(const char* string)
{
	DrawText(string, transform.position.x, transform.position.y, transform.scale, fontColor);
}

void TextRenderer::DrawText(std::string string, const float& xposition, const float& yposition, const float& scale, glm::vec4 fcolor)
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	glm::mat4 projection(1.0f);
	fontShader->Use()
		.SetVector4f("fontColor", fcolor);

	float x = xposition, y = yposition;

	for (auto c = string.begin(); c!= string.end(); c++)
	{
		Character ch = Characters[*c];
		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

#include <imgui.h>
void TextRenderer::Debug(const char* name)
{
	if (ImGui::TreeNode(name))
	{
		if (ImGui::Button("Hot reload: Text Renderer font shader"))
		{
			if (fontShader)
			{
				delete fontShader;

				printf("Initializing static font shader... Screen Dimensions: (%d, %d)\n", Game::AppCam->Width, Game::AppCam->Height);
				fontShader = new Shader("textShader.vertex", "textShader.frag", true);

				glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Game::AppCam->Width), 0.0f, static_cast<float>(Game::AppCam->Height));
				fontShader->Use().SetMatrix4("projection", projection);
			}
		}


		ImGui::ColorEdit4("font color", &fontColor.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
		transform.Debug("Text Renderer: Transform");

		ImGui::TreePop();
	}
}

TextMesh::TextMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TextMesh::~TextMesh()
{
}

void TextMesh::configure()
{
}

void TextMesh::bind()
{
}
