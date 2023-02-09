#include "CanvasItem.h"
#include <imgui.h>
//asume that the shader is already in use....

TextRenderer* CanvasItem::canvasText = nullptr;
glm::vec2 CanvasItem::mousePosition = glm::vec2(0.0f);

void CanvasItem::DrawTexture(Shader* shader, const Transform& _trans)
{
	if (texture) texture->Bind();
}

void CanvasItem::Draw(Shader* shader, Surface* surface, const glm::vec2& offsetPosition)
{
	if (!visible) return;
	Transform t = transform;
	//t.position += glm::vec3(offsetPosition, 0.0f);

	shader->Use()
		.SetMatrix4("model", t.Model())
		.SetVector4f("Color", Color);

	DrawTexture(shader, t);
	surface->Bind();
}

void CanvasItem::Draw(Shader* shader, Surface* surface, const Transform& pt, const glm::vec2& offsetPosition)
{
	if (!visible) return;

	Transform t = transform;
	//t.position += glm::vec3(offsetPosition, 0.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pt.pivot + t.pivot);
	{
		model = glm::translate(model, pt.position + t.position);
		model = glm::rotate(model, pt.rotation, pt.orientation);
		model = glm::scale(model, 
					glm::vec3(
						pt.size.x * pt.scale * pt.direction.x * (t.size.x + offsetPosition.x)* t.scale * t.direction.x,
						pt.size.y * pt.scale * pt.direction.y * (t.size.y + offsetPosition.y)* t.scale * t.direction.y,
						pt.size.z * pt.scale * t.size.z * t.scale
					)
				);
	}
	model = glm::translate(model, -(pt.pivot + t.pivot));
	shader->Use()
		.SetMatrix4("model", model)
		.SetVector4f("Color", Color);
	DrawTexture(shader, t);
	surface->Bind();
}


void CanvasItem::Debug(const char* nodeName)
{
	if (ImGui::TreeNode(nodeName))
	{
		ImGui::ColorEdit4("Color", &Color.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
		transform.Debug("Transform");
		if (texture) texture->RenderDebug();

		ImGui::TreePop();
	}
}






#include "ResourceManager.h"
CanvasText::CanvasText(std::string string, const glm::vec2& position, glm::vec4 color, bool v, bool s)
	: CanvasItem(nullptr, position, color, v, s)
	, Text(string)
	, offset(0.0f)
	, bounds(0.0f)
	, fontColor(glm::vec4(1) - color)
	, viewBounds(false)
	, saveBounds(true)
{
	fontColor.w = 1.0f;
	texture = ResourceManager::GetTexturePtr("default");
	//but actually.... calculate the bounds, make sure that the 
	if (!CanvasItem::canvasText)
		printf("INitialize the canvasText Text Renderer!!!!!!\n");
	else 
	{
		ResizeBounds();
	}
}

CanvasText::~CanvasText()
{
	printf("CanvasItem.cpp::CanvasText::~CanvasText()::\t%s\n", Text.c_str());
}



void CanvasText::ResizeBounds()
{
	//step through text string
	float x = 0, y = 0;
	for (auto c = Text.begin(); c != Text.end(); c++)
	{
		Character ch = CanvasItem::canvasText->Characters[*c];
		float xpos = x + ch.Bearing.x * transform.scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * transform.scale;

		float w = ch.Size.x * transform.scale;
		float h = ch.Size.y * transform.scale;


		x += (ch.Advance >> 6) * transform.scale;
	}
	bounds = glm::vec2(x/60, y);
	transform.pivot.y = -0.25f;
}

bool CanvasText::isPointInBounds(const glm::vec2& pos, Transform* input)
{

	glm::vec2 start, end, mp;
	mp = pos;

	start = glm::vec2(
		input->position.x + transform.position.x,
		input->position.y + transform.position.y
	);
	end = glm::vec2(
		start.x + (bounds.x * input->scale * transform.scale),
		start.y + ((bounds.y + 1) * input->scale * transform.scale)
	);


	bool satisfied = true;
	if (mp.x < start.x || mp.x > end.x) satisfied = false;
	if (mp.y < start.y || mp.y > end.y) satisfied = false;

	return satisfied;
}



void CanvasText::Update(float dt, Transform* input)
{
	if (!selectable) return;
	active = isPointInBounds(CanvasItem::mousePosition, input);
}


void CanvasText::Draw(Shader* shader, Surface* surface, const Transform& pt, const glm::vec2& offsetPosition)
{
	if (!visible) return;

	if(viewBounds)
		CanvasItem::Draw(shader, surface, pt, glm::vec3(bounds, 0.0f));
	glm::vec3 position = pt.position + transform.position + glm::vec3(offsetPosition, 0.0f);
	CanvasItem::canvasText->DrawText(Text, position.x, position.y, transform.scale,active ? this->activeColor : this->fontColor);
}
#include "imguiTextWrap.h"
void CanvasText::Debug(const char* nodeName)
{
	if (ImGui::TreeNode(nodeName))
	{	

		ImGui::Checkbox("visible", &visible);

		ImGui::Separator();
		ImGui::Checkbox("Bounding box visible", &viewBounds);
		ImGui::InputText("Text contents", &Text);
		ImGui::SliderFloat2("bounds", &bounds.x, -100, 100);
		ImGui::ColorEdit4("Color", &Color.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("font color", &fontColor.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
		transform.Debug("Transform");
		if (texture) texture->RenderDebug();

		ImGui::TreePop();
	}
}


void CanvasText::handleCallback()
{
	if (!visible) return;

	if (func) func->evaluate();
	else printf("No callback function found\n");
}