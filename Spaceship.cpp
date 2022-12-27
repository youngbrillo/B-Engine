#include "Spaceship.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


Spaceship::Spaceship(b2World* worldreference, const b2Vec2& spawnPos, b2Body* anchor)
	: thrustForce(0.0f, -50.0f), thrustPosition(0.0f, 3.0f)
	, torqueMag(10.0f)
	, thrust_forward(false), thrust_backward(false), thrust_left(false), thrust_right(false)
	, collisionType(collisionType_astroids::collision_ship)
	, maxCondition(100.0f), maxEnergy(100.0f), condition(100.0f), energy(100.0f)
	, physicalDefense(0.14f)
	, energyCost(0.5)
	, torqueRatio(0.2)
	, operational(true) //can the ship move?
	, broken(false)  //is the ship functionally operational
	, fixture_leftside(NULL), fixture_rightSide(NULL)
{
	mainBody = createSimpleShipBody(worldreference, spawnPos,this, anchor);
}

Spaceship::~Spaceship()
{
	//printf("If you are reading this, make SURE that world destroy body was called before!!!!!\n");
	mainBody->GetWorld()->DestroyBody(mainBody);
	mainBody = NULL;
}

void Spaceship::FixedUpdate(float deltaTime)
{

	if (!operational || broken) return; //change this later, for when powerups n the like come into play, for like no energy thrusting
	b2Vec2 force, position;
	if (thrust_forward) {
		force = mainBody->GetWorldVector(thrustForce);
		position = mainBody->GetWorldPoint(thrustPosition);

		mainBody->ApplyForce(force, position, true);
	}

	if (thrust_backward) {
		force = mainBody->GetWorldVector(-thrustForce);
		position = mainBody->GetWorldPoint(thrustPosition);

		mainBody->ApplyForce(force, position, true);
	}

	if (thrust_right)
	{
		mainBody->ApplyTorque(-torqueMag, true);
	}
	if (thrust_left)
	{
		mainBody->ApplyTorque(torqueMag, true);
	}
}

void Spaceship::Update(float deltaTime)
{
	if (!operational || broken) return;

	if ((thrust_forward || thrust_backward))// && !(thrust_forward && thrust_backward))
	{
		modifyEnergy(-energyCost * deltaTime);
	}

	if ((thrust_left || thrust_right))// && !(thrust_forward && thrust_backward))
	{
		modifyEnergy(-energyCost * deltaTime);
	}
}

void Spaceship::handleBeginContact(b2Contact* contact)
{
}

void Spaceship::handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void Spaceship::handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	if (!operational || broken) return;

	b2Fixture* A = contact->GetFixtureA(); b2Body* AA = contact->GetFixtureA()->GetBody();
	b2Fixture* B = contact->GetFixtureB(); b2Body* BB = contact->GetFixtureB()->GetBody();
	b2Fixture* other = NULL, *myCollider = NULL;

	if (AA != mainBody && BB != mainBody)
		return;

	b2Fixture* itr = mainBody->GetFixtureList();
	while (itr != nullptr)
	{
		if (A == itr)
		{
			if (itr == fixture_leftside) {
				fixture_leftside = A;
			}
			if (itr == fixture_rightSide)
			{
				fixture_rightSide = A;
			}
			myCollider = itr;
			other = B;
		}
		if (B == itr)
		{
			if (itr == fixture_leftside) {
				fixture_leftside = B;
			}
			if (itr == fixture_rightSide)
			{
				fixture_rightSide = B;
			}
			myCollider = itr;
			other = A;
		}
		itr = itr->GetNext();
	}
	
	if (myCollider == NULL || other == NULL)
		return;


	collisionType_astroids* ct = (collisionType_astroids*)other->GetUserData();

	if (!!ct == false) return;


	// Should the body break?
	int32 count = contact->GetManifold()->pointCount;

	float maxImpulse = 0.0f;
	for (int32 i = 0; i < count; ++i)
	{
		maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
	}

	float DM = fixture_leftside->GetDensity() * mainBody->GetMass();
	// subtract from astroids health dir proportional to above comparison
	//if (*ct == collisionType_astroids::collision_astroid && maxImpulse > DM * 40.0f * physicalDefense) 
	if (maxImpulse > DM * 40.0f * physicalDefense) //any hard collision should deal damage
	{
		float dmg= maxImpulse / (DM * 40.0f) * 100;
		modifyCondition(-dmg * physicalDefense);
	//	printf("Collided w/ astroid for %f, %f\n", dmg, dmg*physicalDefense );
	}

	//printf("Astroid post solve:: impact for: (%.2f/%.2f.) health = %.2f.\n", maxImpulse, DM * 40.0f, health);
}

void Spaceship::modifyCondition(float value)
{
	condition += value;
	if (condition > maxCondition) condition = maxCondition;

	if (condition <= 0.0f)
	{
		condition = 0.0f; broken = true;
	}
	else if (broken)
	{
		broken = false;
	}
}

void Spaceship::modifyEnergy(float value)
{
	energy += value;
	if (energy > maxEnergy) energy = maxEnergy;
	
	if (energy <= 0.0f)
	{
		energy = 0.0f; operational = false;
	}
	else if(!operational) { //to get here energy has been established to be > 0
		operational = true;
	}
}

void Spaceship::handleEndContact(b2Contact* contact)
{
}

void Spaceship::onKeyPress(int key)
{
	switch (key)
	{
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			thrust_forward = true;
			break;
		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			thrust_backward = true;
			break;
		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			thrust_left = true;
			break;
		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			thrust_right = true;
			break;
		default:
			break;
	}
}

void Spaceship::onKeyRelease(int key)
{

	switch (key)
	{
	case GLFW_KEY_W:
	case GLFW_KEY_UP:
		thrust_forward = false;
		break;
	case GLFW_KEY_S:
	case GLFW_KEY_DOWN:
		thrust_backward = false;
		break;
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT:
		thrust_left = false;
		break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT:
		thrust_right = false;
		break;
	default:
		break;
	}
}

void Spaceship::Debug()
{
	if (ImGui::TreeNode("- Ship -"))
	{
		ImGui::Checkbox("broken", &broken);
		ImGui::Checkbox("can move", &operational);
		ImGui::Checkbox("forward thrust", &thrust_forward);
		ImGui::Checkbox("backward thrust", &thrust_backward);
		ImGui::Checkbox("left thrust", &thrust_left);
		ImGui::Checkbox("right thrust", &thrust_right);

		ImGui::SliderFloat("condition", &condition, 0, 100.0f);
		ImGui::SliderFloat("defense (phys)", &physicalDefense, 0, 100.0f);
		ImGui::SliderFloat("energy", &energy, 0, 100.0f);
		ImGui::SliderFloat("energy Cost", &energyCost, 0, 100.0f);
		ImGui::SliderFloat("torque ratio", &torqueRatio, 0, 100.0f);

		ImGui::TreePop();
	}
}

b2Body* createSimpleShipBody(b2World* World, const b2Vec2& spawnPos, Spaceship* p_ship, b2Body* ground)
{
	b2Transform xf1;
	xf1.q.Set(0.3524f * b2_pi);
	xf1.p = xf1.q.GetXAxis();

	b2Vec2 vertices[3];
	vertices[0] = b2Mul(xf1, b2Vec2(-1.0f, 0.0f));
	vertices[1] = b2Mul(xf1, b2Vec2(1.0f, 0.0f));
	vertices[2] = b2Mul(xf1, b2Vec2(0.0f, 0.5f));

	b2PolygonShape poly1;
	poly1.Set(vertices, 3);

	b2FixtureDef sd1;
	sd1.shape = &poly1;
	sd1.density = 2.0f;

	b2Transform xf2;
	xf2.q.Set(-0.3524f * b2_pi);
	xf2.p = -xf2.q.GetXAxis();

	vertices[0] = b2Mul(xf2, b2Vec2(-1.0f, 0.0f));
	vertices[1] = b2Mul(xf2, b2Vec2(1.0f, 0.0f));
	vertices[2] = b2Mul(xf2, b2Vec2(0.0f, 0.5f));

	b2PolygonShape poly2;
	poly2.Set(vertices, 3);

	b2FixtureDef sd2;
	sd2.shape = &poly2;
	sd2.density = 2.0f;

	b2BodyDef bd;
	bd.type = b2_dynamicBody;

	bd.position = spawnPos;
	bd.angle = b2_pi;
	bd.allowSleep = false;
	b2Body* newbody = World->CreateBody(&bd);
	p_ship->fixture_leftside = newbody->CreateFixture(&sd1);
	p_ship->fixture_rightSide = newbody->CreateFixture(&sd2);

	p_ship->fixture_leftside->SetUserData(&p_ship->collisionType);
	p_ship->fixture_rightSide->SetUserData(&p_ship->collisionType);

	if (ground)
	{
		attachFrictionJoint(ground, newbody);
	}

	return newbody;
}
