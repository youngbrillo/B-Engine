#pragma once
#include "TextRenderer.h"

#include <tuple>
#include <vector>
class TextManager
{
public://static methods
	static TextManager& IO();
	static void DestroyCurrentInstance();
	//only use one font at a time for right now
	static  std::vector<std::tuple<std::string, std::string>>& GetFonts();
	static int GetFontIndex() { return TextManager::IO().currentFont; }
public://non static methods
	void ChangeFont(const char* fontName);
	void ShiftFont(const int& dist);
	//draw a line to screen detatched from any other lines that may be on the screen
	glm::vec2  DrawText(const char* string, const glm::vec2& position, const float& size = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
	//void DrawText(std::string string, const glm::vec2& position, const float& size = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

	void StartLine(const glm::vec2& screen_pos = glm::vec2(-1.0f), const float& fontSize =1.0f);
	void AddLine(const char* newLine, const glm::vec4& color = glm::vec4(1.0f), bool endLine = false);
	void EndLine();

	
private:
	TextManager();
	~TextManager();
private:
	void DrawLines();
	
private:
	static TextManager* Instance;
	
	TextRenderer* m_textRenderer;
	std::vector<std::tuple<std::string, std::string>> fonts;
	std::vector<std::tuple<std::string, glm::vec4>> Lines;
	int currentFont, lineCount;
	float lineScaling;
	glm::vec2 lineStartingPosition, lineOffset;
private:
	void LoadFonts(const char* fontFileName);
};

