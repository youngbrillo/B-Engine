#include "Sprite.h"



Sprite::Sprite(Texture* texture, const char* jsonFile, int defaultIndex)
	: settings(SpriteSettings())
	, map(new SpriteMap(texture, jsonFile,settings))
	, index(defaultIndex)
	, animStartIndex(0)
	//, animEndIndex(map->elementSize.x * map->elementSize.y)
	, animate(false) //maybe get the animation instructions from the settings?
	, visible(true)
	, color(glm::vec4(1.0f))
	, frameProgress(0.0f)
	, frameLength(100.0f)
{
	//animEndIndex = map->elementSize.x * map->elementSize.y;
	animEndIndex = map->rowcolumnDimensions.x * map->rowcolumnDimensions.y;
}

Sprite::Sprite(Texture* texture, const int columns, const int rows, int defaultIndex)
	: map(new SpriteMap(texture, columns, rows))
	, index(defaultIndex)
	, animStartIndex(0)
	//, animEndIndex(map->elementSize.x* map->elementSize.y)
	, animate(false)
	, visible(true)
	, color(glm::vec4(1.0f))
	, frameProgress(0.0f)
	, frameLength(100.0f)
{
	animEndIndex = map->rowcolumnDimensions.x * map->rowcolumnDimensions.y;
}

Sprite::~Sprite()
{
	delete map;
}

void Sprite::Update(float dt)
{
	if (animate)
	{
		frameProgress += dt;
		int oldIndex = index;
		if (frameProgress >= frameLength / 1000.0f)
		{
			frameProgress = 0.0f;
			index = (index + 1) % animEndIndex;
			if (index < animStartIndex) index = animStartIndex;

			//if(oldIndex != index) 
			map->GetFrame(index);
		}

	}
}

//assume that the model drawing has already been taken care of, 
//now just bind the texture and surface
void Sprite::Draw(Shader* shader)
{

	if (visible)
	{
		shader->SetVector4f("color", this->color);
		map->Draw();
	}

}

#include <imgui.h>

void Sprite::Debug()
{
	if (ImGui::TreeNode("Sprite"))
	{
		ImGui::Checkbox("Visible", &visible);
		ImGui::Checkbox("Animate", &animate);
		ImGui::ColorEdit4("Color", &color.x, ImGuiColorEditFlags_Float);
		ImGui::SliderFloat("Frame Status", &frameProgress, 0, 1.0f);
		ImGui::SliderFloat("Frame Speed", &frameLength, 0.1f, 300.0f);

		ImGui::SliderInt("Frame start", &animStartIndex, 0, animEndIndex);
		ImGui::SliderInt("Frame end", &animEndIndex, 1, map->rowcolumnDimensions.x * map->rowcolumnDimensions.y);

		if (ImGui::SliderInt("Frame Index", &index, 0, animEndIndex))
		{
			map->GetFrame(index);
		}
		static int m[2] = { 0, 0 };

		if (ImGui::SliderInt2("Frame coord", &m[0], 0, map->getrowcolumnDimensions().x))
		{
			map->GetFrame(m[0], m[1]);
		}
		map->Debug("- Sprite Map -");


		ImGui::TreePop();
	}
}
