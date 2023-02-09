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
	printf("Canvas.cpp::\tDestructing Canvas %i\n", (int)children.size());

	for (auto p : children)
	{
		delete p;
	}
	printf("\tCanvas Children left: %i\n", (int)children.size());
	children.clear();
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
