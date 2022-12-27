#include "AstroidObject.h"
#include "AstroidManager.h"

AstroidObject::AstroidObject(GameObjectDefinition* def, void* Creator)
	: GameObject(def)
	, weightClass(size_class::insignifigant)
	, broken(false)
	, fragmentFlag(false)
	, health(100.0f)
	, expireTimeLeft(0.0f)
	, expireTimeOut(0.5f)
	, collisionType(collisionType_astroids::collision_astroid)
	, m_Creator(Creator)
{
	assert(this->enabled_physics == true); //the astroid class doesn't work if phys isn't enabled!

	float _rad = size.x;
	if (_rad >= 2.5f)
	{
		weightClass = size_class::cautionary; //on frag, split in 2
		if (_rad >= 5)
		{
			weightClass = size_class::imposing; //on frag split in 3
		}
	}
	m_contactFixture->SetUserData(&collisionType);
}

AstroidObject::~AstroidObject()
{
	if (m_Creator)
	{
		SpaceBuster::AstroidFactory* homeFactory = (SpaceBuster::AstroidFactory*)m_Creator;
		if (homeFactory)
		{
			homeFactory->handleDelete(this);
		}
		m_Creator = nullptr;
	}

}

void AstroidObject::Update(float dt)
{
	GameObject::Update(dt);
	if (broken)
	{
		expireTimeLeft += dt;
		if (expireTimeLeft >= expireTimeOut)
		{
			deletionFlag = true;
		}

		this->color = glm::vec4(1, 0, 0, 1);
	}


}

void AstroidObject::handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	if (broken || health <= 0.0f) return;
	b2Fixture* A = contact->GetFixtureA();
	b2Fixture* B = contact->GetFixtureB();
	b2Fixture* other;

	if (A != m_contactFixture && B != m_contactFixture) return;

	if (A == m_contactFixture) { other = B; }
	else { other = A; }

	collisionType_astroids* ct = (collisionType_astroids*)other->GetUserData();
	bool canTrackCollision = !!ct;

	if (!canTrackCollision) return;

	// Should the body break?
	int32 count = contact->GetManifold()->pointCount;

	float maxImpulse = 0.0f;
	for (int32 i = 0; i < count; ++i)
	{
		maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
	}

	float DM = m_contactFixture->GetDensity() * m_body->GetMass();
	if (maxImpulse > DM * 40.0f)
	{
		// Flag the body for breaking.
		health -= 100.0f;
	}
	else //then subtract from astroids health dir proportional to above comparison
	{
		if (*ct == collisionType_astroids::collision_bullet)
		{
			health -= maxImpulse / (DM * 40.0f) * 100;
		}
	}

	if (health <= 0.0f) {
		if (weightClass == size_class::insignifigant)
		{
			broken = true;
		}
		else
		{
			fragmentFlag = true;
			if (m_Creator)
			{
				SpaceBuster::AstroidFactory* homeFactory = (SpaceBuster::AstroidFactory*)m_Creator;
				if (homeFactory)
				{
					AstroidState state;
					state.position = m_body->GetPosition();
					state.linearVelocity = m_body->GetLinearVelocity();
					state.angularVelocity = m_body->GetAngularVelocity();
					state.size = b2Vec2(size.x, size.y);

					homeFactory->handleFragment(state, this);
				}
			}
		}
		health == 0.0f;
	}
}
#include "imgui.h"
void AstroidObject::Debug()
{

	std::string a = "-" + name + alias + "-";
	if (ImGui::TreeNode(a.c_str()))
	{
		GameObject::Debug();
		ImGui::Checkbox("Break?", &broken);
		ImGui::Checkbox("Delete?", &this->deletionFlag);

		ImGui::SliderFloat("health", &health, 0, 100.0f);

		ImGui::SliderFloat("destroy timer", &expireTimeLeft, 0.0f, expireTimeOut);
		ImGui::SliderFloat("timer upperlimit", &expireTimeLeft, 0.0f, 2.0f);

		ImGui::TreePop(); 

	}
}
