#include "ParallaxLayer.h"
#include <imgui.h>

ParallaxLayer::ParallaxLayer(Texture* texture, int order, float speed, std::string name, bool _active)
	: layer(new Sprite(texture, 1, 1))
	, visible(true), active(_active)
	, ordinal(order)
	, transitionSpeed(speed)
	, origin(glm::vec2(0.0f))
	, offset(glm::vec2(1.0f))
	, title(name)
{
}

ParallaxLayer::~ParallaxLayer()
{
	delete layer;
}



void ParallaxLayer::Debug()
{
	if (ImGui::TreeNode(title.c_str()))
	{

		ImGui::Checkbox("visible", &visible);
		ImGui::Checkbox("Active", &active);
		ImGui::SliderFloat("Speed", &transitionSpeed,  0.0f, 10.0f);
		
		//origin
		if (ImGui::TreeNode("> Origin & offset point"))
		{
			ImGui::SliderFloat2("pos", &origin.x, -1, 1);
			ImGui::SliderFloat2("offset", &offset.x, -3, 3.0f);
			ImGui::TreePop();
		}

		layer->Debug();
		ImGui::TreePop();
	}
}





// manager class






ParallaxManager::ParallaxManager(ParallaxManagerDefinition* def)
	: transform(def->_transform)
	, direction(def->_direction)
	, baseSpeed(def->baseSpeed)
	, animated(def->animatedByDefault)
	, visible(false)
	, animationSpeed(1.0f)
	, animationProgress(0.0f)
	, animationEnd(100.0f)
{
	transform.UpdateMatrix();

	for (auto i : def->layers)
	{
		layers.emplace_back(i);
	}
}

ParallaxManager::~ParallaxManager()
{
	auto itr = layers.begin();
	while (itr != layers.end())
	{
		auto obj = (*itr);
		//itr = layers.erase(std::remove(layers.begin(), layers.end(), *obj), layers.end());

		itr = layers.erase(std::remove(layers.begin(), layers.end(), obj), layers.end());


		delete obj;
		obj = nullptr;
	}
}

void ParallaxManager::Update(float dt)
{
	if (!animated) return;

	animationProgress += dt;
	if (animationProgress >= animationEnd/1000.0f)
	{
		animationProgress = 0.0f;

		for (auto i : layers)
		{
			if (!i->active) continue;
			i->layer->spriteCoord.x += dt * this->baseSpeed * i->transitionSpeed * this->direction.x;
			i->layer->spriteCoord.y += dt * this->baseSpeed * i->transitionSpeed * this->direction.y;
		}
	}
	}



void ParallaxManager::Draw(Shader* shader, Surface* mesh)
{
	if (!visible) return;
	for (auto i : layers)
	{
		if (!i->visible) continue;
		i->layer->Draw(this->transform.m_model, shader, mesh);
	}
}

void ParallaxManager::Debug(const char* name)
{
	if (ImGui::TreeNode(name))
	{
		ImGui::Checkbox("Visible", &visible);
		ImGui::Checkbox("animate", &animated);

		ImGui::SliderInt2("direction", &direction.x, -1, 1);
		ImGui::SliderFloat("base speed", &baseSpeed, 0, 5);
		ImGui::SliderFloat("anim speed", &animationSpeed, 0, 5.0f);
		ImGui::SliderFloat("anim current", &animationProgress, 0, 1);
		ImGui::SliderFloat("anim end", &animationEnd, 0, 400.0f);
		for (auto i : layers)
		{
			i->Debug();
		}
		transform.Debug("Transform");

		ImGui::TreePop();
	}
}
