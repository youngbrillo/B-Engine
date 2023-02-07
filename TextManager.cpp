#include "TextManager.h"

TextManager* TextManager::Instance = nullptr;
const char* fn = "systemFonts.txt";

std::vector<std::string> get_all_files_names_within_directory(const char* directory);

TextManager& TextManager::IO()
{
	if (!Instance) {
		printf("TextManager::IO::Initializing TextManager Instance\n");
		Instance = new TextManager();
		Instance->LoadFonts(fn);
	}

	return *Instance;
}

void TextManager::DestroyCurrentInstance()
{
	delete Instance;
	Instance = nullptr;
}

std::vector<std::tuple<std::string, std::string>>& TextManager::GetFonts()
{
	return TextManager::IO().fonts;
}

void TextManager::ChangeFont(const char* fontName)
{
	m_textRenderer = new TextRenderer(fontName);
}

void TextManager::ShiftFont(const int& dist)
{
	int attempIndex = this->currentFont + dist, nextIndex = -1;
	if (attempIndex >= fonts.size())
	{
		nextIndex = 0;
	}
	if (attempIndex < 0)
	{
		nextIndex = fonts.size() - 1;
	}


	if (nextIndex != -1)
		currentFont = nextIndex;
	else
		currentFont = attempIndex;

	printf("Changed font index from: %d to: %d (%s)\n", currentFont, nextIndex, std::get<0>(fonts[currentFont]).c_str());

	m_textRenderer = new TextRenderer(std::get<1>(fonts[currentFont]).c_str());
	m_textRenderer->transform.position = glm::vec3(25.0f, 130.0f, 0.0f);
	m_textRenderer->transform.scale = 0.5f;
}

glm::vec2  TextManager::DrawText(const char* string, const glm::vec2& position, const float& size, const glm::vec4& color)
{
	return m_textRenderer->DrawText(string, position.x, position.y, size, color);
}

//void TextManager::DrawText(std::string string, const glm::vec2& position, const float& size, const glm::vec4& color)
//{
//	m_textRenderer->DrawText(string, position.x, position.y, size, color);
//}

void TextManager::StartLine(const glm::vec2& screen_pos, const float& fontSize)
{
	lineStartingPosition = screen_pos;
	Lines.clear();
	lineCount = 0;
	lineScaling = fontSize;
}

void TextManager::AddLine(const char* newLine, const glm::vec4& color, bool endline)
{

	std::string newAddition = newLine;
	if (endline)
		newAddition += "\n";
	else
		newAddition += " ";
	Lines.push_back(std::make_tuple(newAddition, color));
	lineCount++;
}

void TextManager::EndLine()
{
	DrawLines();
	lineStartingPosition = glm::vec2(0.0f);
	Lines.clear();
}

TextManager::TextManager()
	: currentFont(0)
	, lineCount(0)
	, lineStartingPosition(0.0f)
	, lineOffset(0.0f, -36.0f)
	, lineScaling(1.0f)
{
	m_textRenderer = new TextRenderer();
}
TextManager::~TextManager()
{
	delete m_textRenderer;
}

void TextManager::DrawLines()
{
	glm::vec2 position = lineStartingPosition;
	glm::vec2 offset = glm::vec2(0.0f), lastOffset(0.0f);
	glm::vec2 startingPosition = position;


	std::string allLines = "";

	const int arrSize = Lines.size();

	std::vector<glm::vec4> colorarray;
	std::vector<int> colorPositions;
	for (int i = 0; i < Lines.size(); i++)
	{
		allLines += std::get<0>(Lines[0]);
		colorarray.push_back(std::get<1>(Lines[0]));
		colorPositions.push_back(allLines.size() - 1);
	}

	for (int i = 0; i < Lines.size(); i++)
	{
		position += offset;
		std::string line = std::get<0>(Lines[i]);

		position = DrawText(line.c_str(), position, lineScaling, std::get<1>(Lines[i]));
		//position.x += 2.0f;
		//m_textRenderer->DrawText(allLines, position, lineOffset, lineScaling, colorarray, colorPositions);

		if (line[line.size() - 1] == '\n')
		{
			offset = lastOffset;
			offset += lineOffset * lineScaling;
			lastOffset = offset;
			position = startingPosition;
		}
		else {
			offset = glm::vec2(0.0f);
		}
	}
}

void TextManager::LoadFonts(const char* fileName)
{
	currentFont = 0;
	auto vec = get_all_files_names_within_directory("C:/Windows/Fonts/AGENCYB.TTF");

	fonts.push_back(std::make_tuple("OxygenMono-Regular", "./assets/fonts/OxygenMono-Regular.otf"));
	fonts.push_back(std::make_tuple("Dk Hand Regular-orna", "./assets/fonts/DkHandRegular-orna.ttf"));
	fonts.push_back(std::make_tuple("joystix monospace", "./assets/fonts/joystix monospace.otf"));
	fonts.push_back(std::make_tuple("Times New roman", "C:/Windows/Fonts/times.ttf"));
	fonts.push_back(std::make_tuple("Arial", "C:/Windows/Fonts/arial.ttf"));

	for (auto i : vec)
	{
		std::string concat = "C:/Windows/Fonts/" + i;
		fonts.push_back(std::make_tuple(i,  concat));
	}

}

#include <Windows.h>

std::vector<std::string> get_all_files_names_within_directory(const char* directory)
{
	std::vector<std::string> names;
	std::string search_path = directory; 
	search_path += "/*.*";
	WIN32_FIND_DATA fd;
	LPCWSTR path = L"C:/Windows/Fonts/*.ttf";
	HANDLE hFind = ::FindFirstFile(path, &fd);

	//HANDLE hfind2 = FindFirstFile()

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				//names.push_back((const char*)fd.cFileName);


				//names.push_back(std::string(fd.cFileName));
				std::string fileName;
				int l = 260;
				for (int i = 0;i < l; i++)
				{
					fileName += fd.cFileName[i];
				}

				names.push_back(fileName );
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	//printf("files found:%i\n", names.size());

	//for (auto i : names)
	//{
	//	printf("Font named: %s, ", i.c_str());
	//}
	return names;
}
