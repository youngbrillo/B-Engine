#include "Canvas.h"
#include "CanvasItem.h"
#include "Game.h"
Canvas::Canvas()
	: Dimensions (Game::AppCam->Width, Game::AppCam->Height)
	, t()
	, children()
	, rowAlignment(RowAlignment::RowTop)
	, columnAlignment(ColumnAlignment::ColLeft)
{
	t.scale = 50; //for somereason the elements pull up super small
}

Canvas::~Canvas()
{
	auto child = children.begin();
	while (child != children.end())
	{
		auto K = (*child);
		child = children.erase(std::remove(children.begin(), children.end(), K), children.end());
		delete K; K = nullptr;
	}
}

void Canvas::Update(float dt)
{
	for (auto child : children)
	{
		child->Update(dt, &this->t);
	}
}

void Canvas::Draw(Shader* shader, Surface* surface)
{
	for (auto child : children)
	{
		child->Draw(shader, surface, t);
	}
}
#include <imgui.h>
void Canvas::Debug(const char* canvasName)
{
	if (ImGui::TreeNode(canvasName))
	{
		int i = 0;
		t.Debug("Canvas transform");
		if (ImGui::TreeNode("Children"))
		{
			for (auto c : children)
			{
				i++;
				std::string name;
				name = std::to_string(i); name += ": item";
				c->Debug(name.c_str());
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}
