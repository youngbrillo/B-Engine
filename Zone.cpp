#include "Zone.h"

#include "Game.h"
#include "Utility_Physics.h"
using namespace SpaceBuster;

SpaceBuster::Zone::Zone(bool _isCircle, b2Vec2 _position, float maxCapacity, float xExtent, float yExtent, b2FixtureDef* fix)
	: isCircle(_isCircle)
	, size(xExtent, yExtent)
	, capacity(maxCapacity)
	, m_body(NULL)
	, sensor(NULL)
	, max_force(1000.0f), min_force(100.0f), cur_force(1000.0f), deltaForce(50.0f)
	, position(_position)
	, friendly(true)
	, release(false)
	, releaseTimeStart(5.0f), releaseTimeCurrent(5.0f)
{
	b2BodyDef bd;

	bd.position = _position;
	bd.type = b2BodyType::b2_staticBody;
	
	m_body = Game::m_World->CreateBody(&bd);


	if (fix == nullptr)
	{
		basicConstrution(isCircle);
	}
	else
	{
		construct(fix);//sensor = m_body->CreateFixture(fix);
	}
	touchingBodies.clear();
	attractionPoint = m_body->GetLocalCenter();

}


void SpaceBuster::Zone::construct(b2FixtureDef* fd)
{
	if (sensor)
		m_body->DestroyFixture(sensor);
	fd->isSensor = true;
	sensor = m_body->CreateFixture(fd);
}

SpaceBuster::Zone::~Zone()
{
	m_body->DestroyFixture(sensor);
	sensor = NULL;

	Game::m_World->DestroyBody(m_body);
	m_body = NULL;

	touchingBodies.clear();
}

void SpaceBuster::Zone::handleBeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	if (fixtureA != sensor && fixtureB != sensor)
		return;

	b2Fixture* other;
	if (fixtureA == sensor)
		other = fixtureB;
	else
		other = fixtureA;

	if (other->GetBody()->GetType() == b2BodyType::b2_dynamicBody)
	{
		if (touchingBodies.size() < capacity) {
			touchingBodies.push_back(other->GetBody());
		}
	}

	if (!friendly) return; //go no further

	///if captured a ship and is friendly, then gradually turn down the force so that they can get out
	//collisionType_astroids* k = (collisionType_astroids*)other->GetUserData();
	//if (!!k) {
	//	printf("Collision is....: %d\n", *k);
	//	if (*k == collisionType_astroids::collision_ship)
	//	{
	//		release = true;
	//		releaseTimeCurrent = releaseTimeStart;
	//		cur_force = 50.0f;
	//	}
	//}
}

void SpaceBuster::Zone::onPreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	//b/c this is a sensor, it does not get callbacks for this method :(
}

void SpaceBuster::Zone::handleEndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	if (fixtureA != sensor && fixtureB != sensor)
		return;

	b2Fixture* other;
	if (fixtureA == sensor)
		other = fixtureB;
	else
		other = fixtureA;

	if (other->GetBody()->GetType() == b2BodyType::b2_dynamicBody)
	{
		bool foundcontact = std::find(touchingBodies.begin(), touchingBodies.end(), other->GetBody()) != touchingBodies.end();
		if (foundcontact)
		{
			int itr = 0;
			//remove from list
			for (int i = 0; i < touchingBodies.size(); i++)
			{
				if (touchingBodies[i] == other->GetBody())
					itr  = i;
			}
			touchingBodies.erase(touchingBodies.begin() + itr);

		}
	}
}

void SpaceBuster::Zone::FixedUpdate(float dt)
{
	//printf("Center: (%.2f, %.2f)\n", center.x, center.y);
	for (auto i : touchingBodies)
	{
		

		b2CircleShape* circle = (b2CircleShape*)sensor->GetShape();
		b2Vec2 center = m_body->GetWorldPoint(attractionPoint);
		//printf("Center 1: (%.2f, %.2f) - Center 2: (%.2f, %.2f)\n", center.x, center.y, center2.x, center2.y);

		b2Vec2 position = i->GetPosition();

		b2Vec2 d = center - position;
		if (d.LengthSquared() < FLT_EPSILON * FLT_EPSILON)
		{
			continue;
		}

		d.Normalize();
		b2Vec2 F = cur_force * d;
		i->ApplyForce(F, position, false);

	}
}
void SpaceBuster::Zone::Update(float dt)
{
	//shoul make this state dependent but oh well!
	//if (release && cur_force < max_force)
	//{
	//	cur_force -= dt * deltaForce;
	//}
	//else {
	//	cur_force += dt * deltaForce;
	//}

	//if (cur_force > max_force)
	//	cur_force == max_force;


	if (release)
	{
		releaseTimeCurrent -= dt;
		if (releaseTimeCurrent <= 0.0f)
		{
			release = false;
			releaseTimeCurrent = releaseTimeStart;
		}
	}
	else if (!release && cur_force < max_force)
	{
		cur_force += dt * deltaForce;
		if (cur_force > max_force) 
			cur_force == max_force;
	}


}
#include "imgui/imgui.h"
void SpaceBuster::Zone::Debug(const char* _name)
{
	const char* name = (_name == nullptr) ? "Zone" : _name;
	if (ImGui::TreeNode(name))
	{

		ImGui::SliderInt("holding size", &capacity, 0, 100);
		int _capacity = touchingBodies.size();

		ImGui::SliderInt("holding size (current)", &_capacity, 0, 0);

		ImGui::Checkbox("Friendly", &friendly);
		if (friendly)
		{
			ImGui::Checkbox("released?", &release);
			ImGui::SliderFloat("relase cur", &releaseTimeCurrent, 0, releaseTimeStart);
			ImGui::SliderFloat("relase max", &releaseTimeStart, 0, 30);
		}

		if (ImGui::SliderFloat2("position", &position.x, -100, 100, "%.0f"))
		{
			m_body->SetTransform(position, 0);
		}
		if (ImGui::SliderFloat2("physical size", &size.x, 0, 30, "%.0f"))
		{
			basicConstrution(isCircle);
		}
		if (ImGui::Checkbox("is circle", &isCircle))
		{
			basicConstrution(isCircle);
		}
		ImGui::SliderFloat("Force (max)", &max_force, 0.0f, 2000.0f, "%.0f");
		ImGui::SliderFloat("Force (min)", &min_force, 0.0f, 2000.0f, "%.0f");
		ImGui::SliderFloat("Force (cur)", &cur_force, 0.0f, 2000.0f, "%.0f");
		ImGui::SliderFloat("Force (delta)", &deltaForce, 0.0f, 500.0f, "%.0f");
		ImGui::SliderFloat2("center", &attractionPoint.x, -size.x, size.x, "%.0f");

		ImGui::TreePop();
	}
}

void SpaceBuster::Zone::basicConstrution(bool isACircle)
{
	if (isACircle)
	{
		b2FixtureDef fd;
		shape.m_radius = size.x / 2.0f;
		fd.shape = &shape;
		construct(&fd);//sensor = m_body->CreateFixture(&fd);
	}
	else
	{
		b2FixtureDef fd;
		b2PolygonShape shape;
		shape.SetAsBox(size.x / 2.0f, size.y / 2.0f);
		fd.shape = &shape;
		construct(&fd);//sensor = m_body->CreateFixture(&fd);
	}

}

