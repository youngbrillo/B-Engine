#include "ParallaxLayer.h"

ParallaxLayer::ParallaxLayer(Texture* texture, int order, float speed, std::string name)
	: layer(new Sprite(texture, 1, 1))
	, visible(true)
	, ordinal(order)
	, transitionSpeed(speed)
	, a(glm::vec2(1.0f, 1.0f))
	, b(glm::vec2(1.0f, 0.0f))
	, c(glm::vec2(0.0f, 0.0f))
	, d(glm::vec2(0.0f, 1.0f))
	, origin(glm::vec2(0.0f))
	, offset(glm::vec2(1.0f))
	, title(name)
{
}

ParallaxLayer::~ParallaxLayer()
{
	delete layer;
}

void ParallaxLayer::updateVectors(bool autoConfigure)
{
	a = glm::vec2(origin.x + offset.x, 1.0f);
	b = glm::vec2(origin.x + offset.x, 0.0f);
	c = glm::vec2(origin.x, 0.0f);
	d = glm::vec2(origin.x, 1.0f);

	if (autoConfigure)
	{
		printf("----------------Investigate me!!!!!!!!!!!!!!!!");
		//layer->configureTextureCoords(a, b, c, d);
		/*
		* path starts to fall apart here...
		*  -> spritemap.cpp::configureTextureCoords() 
		*  -> -> which generates a new quad surface...
		*/
	}
}

#include <imgui.h>

void ParallaxLayer::Debug()
{
	if (ImGui::TreeNode(title.c_str()))
	{

		ImGui::Checkbox("Active", &visible);
		ImGui::SliderFloat("Speed", &transitionSpeed,  0.0f, 10.0f);
		
		//origin
		if (ImGui::TreeNode("> Origin & offset point"))
		{
			if (ImGui::SliderFloat2("pos", &origin.x, -1, 1))
			{
				this->updateVectors(true);
			}

			if (ImGui::SliderFloat2("offset", &offset.x, -3, 3.0f))
			{
				this->updateVectors(true);
			}
			ImGui::TreePop();
		}

		layer->Debug();
		ImGui::TreePop();
	}
}
