#include "UI_element.h"
#include <imgui.h>

Shader* UIElement::sharedShader = nullptr;
Surface* UIElement::sharedSurface = nullptr;


UIElement::UIElement()
	: position(glm::vec3(0.0f))
	, pivot(glm::vec3(0.0f))
	, size(glm::vec3(1.0f))
	, color(glm::vec4(1.0f))
	, scale(1.0f), rotation(0)
	, name("UI Element base constructor'd")
{
}

UIElement::UIElement(const UIElementDefinition* def)
	: position(def->position)
	, pivot(def->pivot)
	, size(def->size)
	, color(def->color)
	, scale(def->scale)
	, rotation(def->rotation)
	, name(def->elementName)
	, outlineColor(def->outlineColor)
	, backgroundColor(def->backgroundColor)
{
}

UIElement::~UIElement()
{

}

void UIElement::Debug()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::ColorEdit4("Color", &color.x, ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("bg Color", &backgroundColor.x, ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("outline Color", &outlineColor.x, ImGuiColorEditFlags_Float);
		//ImGui::Checkbox("Enabled", &enabled);
		//ImGui::Checkbox("visible", &visible);

		ImGui::SliderFloat3("Position", &position.x, -50, 50);
		ImGui::SliderFloat3("pivot", &pivot.x, -0.5, 0.5);
		ImGui::SliderFloat3("size", &size.x, 0, 5);
		ImGui::SliderFloat("scale", &scale, 0, 5);


		ImGui::SliderAngle("rotation angle", &rotation);

		ImGui::TreePop();
	}
}
