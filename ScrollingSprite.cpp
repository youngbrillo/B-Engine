#include "ScrollingSprite.h"

ScrollingSprite::ScrollingSprite(const ScrollingSpriteDefinition* def)
	: position(def->position)
	, pivot(def->pivot)
	, size(def->size)
	, scale(def->scale)
	, mainColor(def->mainColor)
	, visible(true)
{
	init(*def);
}

void ScrollingSprite::init(const ScrollingSpriteDefinition& def)
{
	for (int i = 0; i < def.layerCount; i++)
	{
		layers.emplace_back(def.layers[i]);
		float scrollfactor = (i + 1);
		if (i > 0) scrollfactor* def.layerScrollModifier;

		scrollSpeed.push_back(def.baseScrollSpeed / scrollfactor);
		drawOrder.push_back(i);
	}
}

ScrollingSprite::~ScrollingSprite()
{
	layers.clear();
	scrollSpeed.clear();
	drawOrder.clear();
}

void ScrollingSprite::Update(float deltaTime)
{
	//if (!visible) return;
}

void ScrollingSprite::Draw(Shader* shader, Surface* surface)
{
	if (!visible) return;

	glm::mat4 model(1.0f);
	model = glm::translate(model, pivot);
	model = glm::translate(model, position);
	model = glm::scale(model, size * scale);
	model = glm::translate(model, -pivot);


	shader->SetMatrix4("model", model);
	for (int i = 0; i < drawOrder.size(); i++)
	{
		layers[drawOrder[i]]->Bind();
		surface->Bind();
	}

	
}
#include <imgui.h>
#include <sstream>

void ScrollingSprite::Debug()
{
	if (ImGui::TreeNode("Parallax Sprite"))
	{
		ImGui::SliderFloat3("pos", &position.x, -10, 10);
		ImGui::SliderFloat3("pivot", &pivot.x, -0.5, 0.5);
		ImGui::SliderFloat3("size", &size.x, 0.1, 10);
		ImGui::SliderFloat("scale", &scale, 0.1, 10);

		int index = 0, jdex = 0;
		for (auto i : layers)
		{
			std::stringstream textureName;
			textureName << "Texure: " << index;

			if (ImGui::TreeNode(textureName.str().c_str()))
			{
				i->RenderDebug();
				ImGui::TreePop();
			}

			index++;
		}
		for (auto i : scrollSpeed)
		{
			std::stringstream speedName;
			speedName << "parallax Speed: " << jdex;

			ImGui::SliderFloat(speedName.str().c_str(), &i, 0.0f, 10.0f );
			jdex++;
		}

		ImGui::TreePop();
	}

}
