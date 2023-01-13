#include "SpriteMap.h"


SpriteMap::SpriteMap(Texture* texture, const char* jsonFile, SpriteSettings& settings)
	: TextureReference(texture)
	, a(glm::vec2(1, 1))
	, b(glm::vec2(1, 0))
	, c(glm::vec2(0, 0))
	, d(glm::vec2(0, 1))
	, elementSize(0, 0)
	, m_surface(nullptr)
	, rowcolumnDimensions(glm::i32vec2(0,0))
	, RenderLazy(true)
{
	m_surface = new quadSurface();
	m_surface->Generate(a, b, c, d);

	k_surface->Generate(a, b, c, d);

	configFromJson(jsonFile, settings);
}

SpriteMap::SpriteMap(Texture* texture, const int columns, const int rows)
	: TextureReference(texture)
	, a(glm::vec2(1, 1))
	, b(glm::vec2(1, 0))
	, c(glm::vec2(0, 0))
	, d(glm::vec2(0, 1))
	//, elementSize(texture->width / columns, texture->height / columns)
	, elementSize(1/(1.0f * columns), 1/ (1.0f * rows))
	, m_surface(nullptr)
	, rowcolumnDimensions(glm::i32vec2(columns, rows))
	, RenderLazy(true)
{
	m_surface = new quadSurface();
	m_surface->Generate(a, b, c, d);

	k_surface = new experimentalSurface();
	k_surface->Generate(a, b, c, d);
}

SpriteMap::~SpriteMap()
{
	TextureReference = nullptr;
	if(m_surface)
		delete m_surface;

	delete k_surface;
}

void SpriteMap::GetFrame(const int row, const int column)
{
	float top = elementSize.y * (column + 1);
	float bottom = (1.0f* column) / (1.0f * rowcolumnDimensions.y);
	float right = elementSize.x * (row + 1);
	float left = (1.0f * row) /(1.0f * rowcolumnDimensions.x);

	a = glm::vec2(right, top);
	b = glm::vec2(right, bottom);
	c = glm::vec2(left, bottom);
	d = glm::vec2(left, top);

	configureTextureCoords();
}

void SpriteMap::GetFrame(const int _index)
{
	int x = 0, y = 0;
	//x = index % (int)elementSize.x;
	x = _index % (int)rowcolumnDimensions.x;
	//y = (int)elementSize.y - 1 - index / (int)elementSize.x; // subtract from the top, b/c the origin point is (0,0)
	y = (int)rowcolumnDimensions.y - 1 - _index / (int)rowcolumnDimensions.x; // subtract from the top, b/c the origin point is (0,0)

	GetFrame(x, y);
}


void SpriteMap::configureTextureCoords()
{
	if (RenderLazy) {
		if (m_surface) {
			delete m_surface;
			m_surface = new quadSurface();
		}
		m_surface->Generate(a, b, c, d);
	}
	else
	{
		k_surface->UpdateVerticies(a, b, c, d);
	}
}
void SpriteMap::configureTextureCoords(glm::vec2 aa, glm::vec2 bb, glm::vec2 cc, glm::vec2 dd)
{
	a = aa;
	b = bb;
	c = cc;
	d = dd;
	configureTextureCoords();
}
#include <imgui.h>

void SpriteMap::Debug(const char* name)
{
	ImGui::Checkbox("Lazy rendering", &RenderLazy);
	if (ImGui::TreeNode(name))
	{

		if (ImGui::SliderFloat2("Top right", &a.x, 0, 1))
		{
			configureTextureCoords();
		}
		if (ImGui::SliderFloat2("bot right", &b.x, 0, 1))
		{
			configureTextureCoords();
		}
		if (ImGui::SliderFloat2("bot left", &c.x, 0, 1))
		{
			configureTextureCoords();
		}
		if (ImGui::SliderFloat2("top left", &d.x, 0, 1))
		{
			configureTextureCoords();
		}



		TextureReference->RenderDebug();
		ImGui::TreePop();
	}
}

void SpriteMap::Draw() // less of a 'draw' more of a bind
{
	TextureReference->Bind();
	if(RenderLazy)
		m_surface->Bind();
	else
		k_surface->Bind();
}

void SpriteMap::Draw(glm::vec2 aa, glm::vec2 bb, glm::vec2 cc, glm::vec2 dd)
{
	configureTextureCoords(aa, bb, cc, dd);
	Draw();
}

void SpriteMap::configFromJson(const char* jsonFile, SpriteSettings& settings)
{

}
