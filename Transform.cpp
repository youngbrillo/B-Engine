#include "Transform.h"
#include <glm/glm.hpp>

Transform::Transform()
	: position(glm::vec3(0.0f))
	, size(glm::vec3(1.0f))
	, pivot(glm::vec3(-0.5f, -0.5f, 0.0f))
	, orientation(glm::vec3(0.0f, 0.0f, 1.0f))
	, m_model(glm::mat4(1.0f))
	, rotation(0.0f)
	, scale(1.0f)
	, direction(1)
{
}


Transform::~Transform()
{
}

glm::mat4& Transform::Model()
{
	UpdateMatrix();

	return m_model;
}

void Transform::UpdateMatrix()
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, pivot);
	{
		m_model = glm::translate(m_model, position);
		m_model = glm::rotate(m_model, rotation, orientation);
		m_model = glm::scale(m_model, glm::vec3(size.x * scale * direction.x, size.y * scale * direction.y, size.z * scale) );
	}
	m_model = glm::translate(m_model, -pivot);
}

#include <imgui.h>
void Transform::Debug(const char* owner)
{
	if (ImGui::TreeNode(owner))
	{
		if (ImGui::SliderFloat3("pos", &position.x, -10, 10)) { UpdateMatrix(); }
		if (ImGui::SliderFloat3("pivot", &pivot.x, -0.5, 0.5)) {

			position += pivot;
			position -= pivot;
			UpdateMatrix();
		}
		if (ImGui::SliderAngle("rotation", &rotation)) { UpdateMatrix(); }
		if (ImGui::SliderFloat3("orientation", &orientation.x, -1, 1)) { UpdateMatrix(); }
		if (ImGui::SliderFloat("scale", &scale, 0.01, 10)) { UpdateMatrix(); }
		if (ImGui::SliderFloat3("size", &size.x, -10, 10)) { UpdateMatrix(); }
		if (ImGui::SliderInt2("direction", &direction.x, -1, 1)) { UpdateMatrix(); }

		ImGui::TreePop();
	}

}
