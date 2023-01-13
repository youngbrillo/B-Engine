#include "ScrollingSprite.h"
#include <sstream>

ScrollingSprite::ScrollingSprite(const ScrollingSpriteDefinition* def)
	: position(def->position)
	, pivot(def->pivot)
	, size(def->size)
	, scale(def->scale)
	, mainColor(def->mainColor)
	, visible(true)
	, zOffset(0.01f)
	, direction(0)
	, speedMultiplier(def->speedMultiplier)
	, name(def->name)
{
	init(*def);
}

void ScrollingSprite::init(const ScrollingSpriteDefinition& def)
{
	for (int i = 0; i < def.layers.size(); i++)
	{
		float scrollfactor = (i + 1);
		if (i > 0) scrollfactor* def.layerScrollModifier;
		std::stringstream ss;
		ss << "P. Layer: " << i;
		layers.emplace_back(new ParallaxLayer(def.layers[i], i, def.baseScrollSpeed/scrollfactor, ss.str()));
	}
}

ScrollingSprite::~ScrollingSprite()
{
	//actually delete..
	auto iter = layers.begin();
	while (iter != layers.end())
	{
		auto obj = (*iter);
		iter = layers.erase(std::remove(layers.begin(), layers.end(), (*iter)), layers.end());
		delete obj;
	}

	layers.clear();

	printf("Deleted Parallax Object Instance\n");
}

void ScrollingSprite::Update(float deltaTime)
{
	if (direction.x == 0) return; //todo: account for horizontal scroll!!!
	int i = 0;
	for (auto layer : layers)
	{
		if (!layer->visible) continue;

		float value = layer->origin.x;
		if (direction.x < 0)
		{
			value -= deltaTime * layer->transitionSpeed * speedMultiplier;
		}
		else
		{
			value += deltaTime * layer->transitionSpeed * speedMultiplier;
		}

		layer->origin.x = value;
		layer->updateVectors(true);
		
		i++;
	}
}

void ScrollingSprite::Draw(Shader* shader)
{
	if (!visible) return;

	glm::mat4 model(1.0f);
	glm::vec3 localpos = position;

	//todo: sort later :/

	for (int i = 0; i < layers.size(); i++)
	{
		if (!layers[i]->visible) { continue; }
		localpos.z += zOffset;
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pivot);
			model = glm::translate(model, localpos);
			model = glm::scale(model, size * scale);
			model = glm::translate(model, -pivot);
		}
		shader->SetMatrix4("model", model);
		layers[i]->layer->Draw();
	}

	
}
#include <imgui.h>

void ScrollingSprite::Debug()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::SliderInt("Direction", &direction.x, -1, 1);
		ImGui::SliderFloat("speed adjust", &speedMultiplier, 0.01, 1.0f);

		ImGui::SliderFloat3("pos", &position.x, -10, 10);
		ImGui::SliderFloat3("pivot", &pivot.x, -0.5, 0.5);
		ImGui::SliderFloat3("size", &size.x, 0.1, 10);
		ImGui::SliderFloat("scale", &scale, 0.1, 10);

		int index = 0, jdex = 0;
		for (auto i : layers)
		{
			i->Debug();
		}
		ImGui::TreePop();
	}
}

