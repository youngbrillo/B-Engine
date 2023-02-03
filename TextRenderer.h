#pragma once
#include <glm/glm.hpp>
#include "Transform.h"
#include <ft2build.h>
#include FT_FREETYPE_H  
#include <map>

#include "Shader.h"

struct Character
{
	unsigned int	TextureID;	// id for the glyph texture
	glm::ivec2		Size;		// size of glyph
	glm::ivec2		Bearing;	// offset from baseline to top-left of glyph
	unsigned int	Advance;	// offset to advance to next glyph
};

class TextMesh
{
public:
	TextMesh();
	~TextMesh();

	void configure();
	void bind();
public:
	unsigned int VAO, VBO;
};

class TextRenderer
{
public:
	TextRenderer(const char* filepath = "./assets/fonts/DkHandRegular-orna.ttf");

	~TextRenderer();
	void LoadFont(const char* filepath);
	void DrawText(const char* string);
	void DrawText(std::string string, const float& xposition, const float& yposition, const float& scale, glm::vec4 color = glm::vec4(1.0f));

	void Debug(const char* name = "Text renderer debug");
public:
	Transform transform;
	glm::vec4 fontColor;

	std::map<char, Character> Characters;

	static Shader* fontShader;
	unsigned int VAO, VBO;
};

