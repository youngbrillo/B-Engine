#include "GameObject.h"
#include "Game.h"

Surface* GameObject::g_GameObjectSurface = nullptr;
Shader* GameObject::g_gameObjectShader = nullptr;
unsigned int GameObject::Count = 0;
#include <sstream>

GameObject::GameObject(GameObjectDefinition* def)
	: position(def->position)
	, size(def->size)
	, orientation_axis(def->orientation_axis)
	, rotation_axis(def->rotation_axis)
	, scale(def->scale)
	, orientation(def->orientation)
	, rotation(def->rotation)
	, enabled(def->enabled)
	, enabled_physics(def->enabled_physics)
	, visible(def->visible)
	, name(def->name.c_str())
	, color(def->color)
	, linearVelocity(def->linearVelocity)
	, angularVelocity(0.0f)
	, m_body(NULL)
	, m_contactFixture(NULL)
	, m_sprite(def->texture)
	, m_surface(NULL)
	, deletionFlag(false)
	, shapeType(b2Shape::Type::e_circle)
	, Id(Count++)
{	
	//set alias
	std::stringstream ss; ss << Id; alias = ss.str().c_str();

	if (enabled_physics)
	{
		if (def->objBodydef)
		{
			m_body = Game::m_World->CreateBody(def->objBodydef);
			if (def->objFixDef) 
			{
				m_contactFixture = m_body->CreateFixture(def->objFixDef);
				shapeType = def->objFixDef->shape->GetType();
			}
		}
		else {
			//use default body and shape 
			b2BodyDef bd;
			bd.type = b2BodyType::b2_dynamicBody;
			bd.position = b2Vec2(position.x, position.y);
			bd.linearVelocity = def->linearVelocity;
			bd.fixedRotation = false;
			bd.angle = orientation;

			m_body = Game::m_World->CreateBody(&bd);

			b2FixtureDef fd;
			fd.restitution = 0.0f;
			fd.density = 1.0f;
				b2PolygonShape shape;
				shape.SetAsBox(size.x/2 * scale, size.x/2 * scale);
				fd.shape = &shape;

				shapeType = b2Shape::Type::e_polygon;
			m_contactFixture = m_body->CreateFixture(&fd);
		}
	}
}


GameObject::~GameObject()
{
	if (m_body != NULL)
		Game::m_World->DestroyBody(m_body);


	m_body = NULL;
	m_contactFixture = NULL;
	m_sprite = NULL;
	m_surface = NULL;
}

void GameObject::FixedUpdate(float dt)
{
	if (!enabled_physics || !enabled) return;

}

void GameObject::Update(float dt)
{
	if (!enabled) return;


	if (enabled_physics && m_body)
	{
		position.x = m_body->GetPosition().x;
		position.y = m_body->GetPosition().y;
		position.z = 0.0f;
		orientation = m_body->GetAngle();

		linearVelocity = m_body->GetLinearVelocity();
	}
}

void GameObject::Draw(Shader* shader, Surface* surface)
{
	if (!visible) return;
		//add the render code here, I wanna see some images!!!

	glm::mat4 model(1);
	model = glm::translate(model, this->position);
	model = glm::rotate(model, orientation, orientation_axis); //rotate obj around it's personal orientation //self
	//model = glm::rotate(model, rotation, rotation_axis);		//rotate the rotation around another axis  //wip
	model = glm::scale(model, size * scale);


	Shader* _shad = shader != nullptr ? shader : g_gameObjectShader;
	Surface* _surf = surface != nullptr ? surface : g_GameObjectSurface;

	if (_shad) 
	{
		_shad->Use()
			.SetMatrix4("model", model)
			.SetVector4f("color", this->color);
	}
	if (this->m_sprite) 
	{
		m_sprite->Bind();
	}

	if(_surf) _surf->Bind();
}

#include "imgui.h"

void GameObject::Debug()
{
	std::string a = name + "<" + alias + ">";
	if (ImGui::TreeNode(a.c_str()))
	{
		ImGui::ColorEdit4("Color", &color.x, ImGuiColorEditFlags_Float);
		ImGui::Checkbox("Enabled", &enabled);
		ImGui::Checkbox("visible", &visible);
		ImGui::Checkbox("physical?", &enabled_physics);
		ImGui::Checkbox("delete?", &deletionFlag);


		if (ImGui::TreeNode("Transform"))
		{
			ImGui::SliderFloat3("Position", &position.x, 0, 5);
			if (ImGui::SliderFloat3("size", &size.x, 0, 5))
			{
				ResizePhys();
			}
			if (ImGui::SliderFloat("scale", &scale, 0, 5))
			{
				ResizePhys();
			}

			ImGui::SliderAngle("orientation", &orientation);
			ImGui::SliderFloat3("orientation axis", &orientation_axis.x, 0, 1);

			ImGui::SliderAngle("rotation angle", &orientation);
			ImGui::SliderFloat3("rotate about axis", &rotation_axis.x, 0, 1);

			ImGui::TreePop();
		}

		if (this->m_sprite) {
			m_sprite->RenderDebug();
		}
		//if (enabled) {
		//	if (ImGui::TreeNode("Physics"))
		//	{
		//		ImGui::TreePop();
		//	}
		//}
		ImGui::TreePop();
	}
}

void GameObject::ResizePhys()
{
	if (m_body == NULL || m_contactFixture == NULL) {
		printf("Returning early from resize b/c the body or contact fixture is missing\n");
		return;
	}
	m_body->DestroyFixture(m_contactFixture);
	m_contactFixture = nullptr;


	b2FixtureDef fd;
	fd.isSensor = true;
	if (shapeType == b2Shape::Type::e_circle)
	{
		b2CircleShape shape;
		shape.m_radius = size.x * scale /2;
		fd.shape = &shape;

		m_contactFixture = m_body->CreateFixture(&fd);
	}
	else if (shapeType == b2Shape::Type::e_polygon)
	{
		b2PolygonShape shape;
		shape.SetAsBox(size.x * scale/2, size.y * scale/2);
		fd.shape = &shape;
		m_contactFixture = m_body->CreateFixture(&fd);
	}
}


//utility function
void GameObject::AddPhysics(b2BodyDef*, b2FixtureDef*)
{
	printf("GameObject.cpp::AddPhysics::\tWoops!, add functionality later!\n");
}
