#include "Sprite.h"

Sprite::Sprite()
	: transform(Transform())
	, Color(glm::vec4(1.0f))
	, index(0)
	, spriteSize(1.0f)
	, dimensions(1.0, 1.0f)
	, texture(nullptr)
	, animated(false)
	, spriteCoord(0.0f)
	, visible(true)
{
}

Sprite::Sprite(Texture* t, int columns, int rows)
	: transform(Transform())
	, Color(glm::vec4(1.0f))
	, index(0)
	, spriteSize(1/(float)columns, 1/(float)rows)
	, dimensions(columns, rows)
	, texture(t)
	, animated(false)
	, frameStart(0)
	, frameEnd(columns * rows)
	, maxFrame(columns * rows)
	, frame_time_length(100.0f)
	, frame_time_remaining(0.0f)
	, visible(true)
{
}

Sprite::~Sprite()
{
	texture = nullptr;
}

void Sprite::Update(float dt)
{
	if (animated)
	{
		frame_time_remaining += dt;
		if (frame_time_remaining >= frame_time_length / 1000.0f)
		{
			frame_time_remaining = 0.0f;
			index = (index + 1) % frameEnd;
			if (index < frameStart) index = frameStart;

			spriteCoord = getCoordinate(index);
		}
	}
}

void Sprite::Draw(Shader* shader, Surface* mesh)
{
	if (!visible) return;
	shader->SetMatrix4("model", transform.m_model)
	.SetVector4f("color", Color)
	.setInt("index", index)
	.SetVector2f("spriteSize", spriteSize)
	.SetVector2f("spriteCoord", spriteCoord)
	.SetVector2f("sheetdimensions", dimensions);

	texture->Bind();
	mesh->Bind();
}

void Sprite::Draw(glm::mat4 model, Shader* shader, Surface* mesh)
{
	if (!visible) return;
	shader->SetMatrix4("model", model)
		.SetVector4f("color", Color)
		.SetVector2f("spriteSize", spriteSize)
		.SetVector2f("sheetdimensions", dimensions)
		.SetVector2f("spriteCoord", spriteCoord);

	texture->Bind();
	mesh->Bind();
}

#include <imgui.h>
void Sprite::Debug(const char* name)
{
	std::string title = name ? name : "Sprite";
	if (ImGui::TreeNode(title.c_str()))
	{
		ImGui::ColorEdit4("Color", &Color.x, ImGuiColorEditFlags_Float);
		ImGui::SliderFloat2("sprite dimensions", &spriteSize.x, 0, 1.0f);
		ImGui::SliderFloat2("rows and columns", &dimensions.x,0, 30);

		ImGui::Checkbox("Visible", &visible);
		ImGui::Checkbox("Animate", &animated);

		if (ImGui::TreeNode("Animation"))
		{
			ImGui::SliderFloat("Frame Status", &frame_time_remaining, 0, frame_time_length/1000.0f);
			ImGui::SliderFloat("Frame Speed", &frame_time_length, 0.1f, 300.0f);

			ImGui::SliderInt("Frame start", &frameStart, 0, frameEnd);
			ImGui::SliderInt("Frame end", &frameEnd, 1, maxFrame);
			if (ImGui::SliderInt("Index", &index, 0, maxFrame))
			{
				spriteCoord = getCoordinate(index);
			}

			ImGui::SliderInt("maxium Frame", &maxFrame, 0, 200);
			ImGui::SliderFloat2("current coord", &spriteCoord.x, 0, 1);
			ImGui::TreePop();
		}
		transform.Debug("Transform");
		texture->RenderDebug();

		ImGui::TreePop();
	}
}

glm::vec2 Sprite::getCoordinate(const int i)
{
	float x, y;
	x = i % (int)dimensions.x;
	y = i / (int)dimensions.y;

	x = spriteSize.x * x;
	y = spriteSize.y * ((int)dimensions.y - 1 - y);
	return glm::vec2(x,y);
}
