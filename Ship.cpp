#include "Ship.h"
#include "Game.h"
Ship::Ship(ShipAttributeDef* def, b2World* World, const b2Vec2& spawnPos, b2Body* anchor)
	: attributes(ShipAttributes(def))
	, collisionType(collisionType_astroids::collision_ship)
	, targetAngle(0.0f)
	, target(b2Vec2(0.0f, 0.0f))
	, rotationSpeed(1.0f/3.0f)
	, groundBodyRef(anchor)
	, turnjointref(nullptr)
	, thrust_forward(false), thrust_backward(false), thrust_left(false),thrust_right(false)
	, maximum_speed(30.0f)
	, current_speed(0.0f)
	, constrain_velocity(true)
	, doubleTapIntervalLimit(0.5f), doubleTapIntervalTimeLeft(0.0f)
	, doubleTappedForward(false), doubleTappedBackward(false), doubleTappedLeft(false), doubleTappedRight(false)
	, listeningForDoubleTap(false), doubleTapEnabled(false)
	, lastKeyDown(0), lastKeyUp(0), doubleTapKey(1)
	, doubleTappVelocityMod(1.0f)//, doubleTappTorqueMod(1.1f)
	, physicalDefense(1.0f)// 0.14f)
{
	//create the default arrow body
#if 1
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
		b2Fixture* newFixture;
		{
			newFixture = newbody->CreateFixture(&sd1);
			newFixture->SetUserData(&this->collisionType);
		}
		{
			newFixture = newbody->CreateFixture(&sd2);
			newFixture->SetUserData(&this->collisionType);
		}

		if (groundBodyRef)
		{
			turnjointref = attachFrictionJoint2(anchor, newbody);
		}
		mBody = newbody;
	}
#else
	{
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody;

		//hexagonal shape definition
		b2PolygonShape polygonShape;
		b2Vec2 vertices[6];
		for (int i = 0; i < 6; i++) {
			float angle = -i / 6.0 * 360 * b2_pi/180 ;
			vertices[i].Set(sinf(angle), cosf(angle));
		}
		vertices[0].Set(0, 4); //change one vertex to be pointy
		polygonShape.Set(vertices, 6);

		//fixture definition
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 1;

		//create dynamic body
		myBodyDef.position.Set(0, 10);
		mBody = World->CreateBody(&myBodyDef);
		mBody->CreateFixture(&myFixtureDef);

		//zero gravity
		attributes.ThrustForce.y = 100.0f;

		if (anchor && false)
		{
			attachFrictionJoint(anchor, mBody);
			attributes.TorqueMag = 100.0f;
		}
	}
#endif

}

Ship::~Ship()
{
	Game::m_World->DestroyBody(mBody);
	mBody = nullptr;
}
float DEGTORAD = b2_pi / 180.0f;
void Ship::FixedUpdate(float deltaTime)
{

	if (attributes.broken || !attributes.operational) return;
	current_speed = mBody->GetLinearVelocity().Length();
	b2Vec2 force, position;

	if (thrust_forward) {
		force = mBody->GetWorldVector(attributes.ThrustForce);
		position = mBody->GetWorldPoint(attributes.ThrustPosition);


		if (constrain_velocity && current_speed > maximum_speed)
		{
			force = b2Vec2_zero;
		}
		mBody->ApplyForce(force, position, true);
	}

	if (thrust_backward) {
		force = mBody->GetWorldVector(-attributes.ThrustForce);
		position = mBody->GetWorldPoint(attributes.ThrustPosition);

		if (constrain_velocity && current_speed > maximum_speed)
		{
			force = b2Vec2_zero;
		}
		mBody->ApplyForce(force, position, true);
	}

	if (thrust_right)
	{
		mBody->ApplyTorque(-attributes.TorqueMag, true);
	}
	if (thrust_left)
	{
		mBody->ApplyTorque(attributes.TorqueMag, true);
	}

	if (turn1) {
		float nextAngle = mBody->GetAngle() + mBody->GetAngularVelocity() * rotationSpeed;
		//float nextAngle = mBody->GetAngle() + mBody->GetAngularVelocity() / 60.0;

		//mBody->ApplyTorque(totalRotation < 0 ? -attributes.TorqueMag : attributes.TorqueMag, true);

		float totalRotation = targetAngle - nextAngle;
		while (totalRotation < -180 * DEGTORAD) totalRotation += 360 * DEGTORAD;
		while (totalRotation > 180 * DEGTORAD) totalRotation -= 360 * DEGTORAD;
		float desiredAngularVelocity = totalRotation * 60;
		float change = 1 * DEGTORAD; //allow 1 degree rotation per time step
		desiredAngularVelocity =  std::min(change, std::max(-change, desiredAngularVelocity));

		float impulse = mBody->GetInertia() * desiredAngularVelocity;
		mBody->ApplyAngularImpulse(impulse, true);
		

	}


}

void Ship::ApplyImpulseForce()
{

	if (attributes.broken || !attributes.operational) return;
	float dir = 0;
	bool vertMovement = false;
	dir = (doubleTapKey == GLFW_KEY_W || doubleTapKey == GLFW_KEY_UP || doubleTapKey == GLFW_KEY_RIGHT || doubleTapKey == GLFW_KEY_D)
		? -1 : 1;


	if (doubleTapKey == GLFW_KEY_W || doubleTapKey == GLFW_KEY_UP || doubleTapKey == GLFW_KEY_S || doubleTapKey == GLFW_KEY_DOWN)
	{
		vertMovement = true;
	}

	if (doubleTapKey == GLFW_KEY_LEFT || doubleTapKey == GLFW_KEY_A || doubleTapKey == GLFW_KEY_RIGHT || doubleTapKey == GLFW_KEY_D)
	{
		vertMovement = false;
	}
	b2Vec2 _thrustForce = { 0.0f * dir, -1 * dir * attributes.ThrustForce.y };
	b2Vec2 force = mBody->GetWorldVector(_thrustForce);
	b2Vec2 position = mBody->GetWorldPoint(attributes.ThrustPosition);

	//mainBody->ApplyForce(force, position, true);
	b2Vec2 impulse = { force.x * doubleTappVelocityMod, force.y * doubleTappVelocityMod };
	b2Vec2 swappedInpulse = { impulse.y, impulse.x };
	b2Vec2 swappedThrust = { _thrustForce.y, _thrustForce.x };

	if (vertMovement)
	{
		//this->mainBody->ApplyLinearImpulse(impulse, position, true);
		mBody->ApplyLinearImpulseToCenter(impulse, true);
	}
	else {
		//this->mainBody->ApplyLinearImpulse({impulse.y, impulse.x}, position, true);
		//the_left_and_right_thrusting_needs_some_tweaking...
		mBody->ApplyLinearImpulseToCenter(swappedInpulse, true);
	}
	//printf("Applying impluse force of (%.2f,%.2f)\n", impulse.x, impulse.y);
	//modifyEnergy(-1 * energyCost * impulseCostModifier);
	attributes.UpdateEnergy(-3.0f);
}
void Ship::Update(float deltaTime)
{

	if (listeningForDoubleTap)
	{
		doubleTapIntervalTimeLeft += deltaTime;
		if (doubleTapIntervalTimeLeft >= doubleTapIntervalLimit)
		{
			listeningForDoubleTap = false;
			doubleTapIntervalTimeLeft = 0.0f;
		}
	}

	if (thrust_forward || thrust_backward || thrust_left || thrust_right) {
		attributes.UpdateEnergy(-0.33 * deltaTime);
	}

}
bool Ship::handleBeginContact(b2Contact* contact)
{

	 b2Body* AA = contact->GetFixtureA()->GetBody();
	 b2Body* BB = contact->GetFixtureB()->GetBody();

	if (AA != mBody && BB != mBody)
		return false;

}
void Ship::handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold){}
void Ship::handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

	if (!attributes.operational ||attributes.broken) return;

	b2Fixture* A = contact->GetFixtureA(); b2Body* AA = contact->GetFixtureA()->GetBody();
	b2Fixture* B = contact->GetFixtureB(); b2Body* BB = contact->GetFixtureB()->GetBody();
	b2Fixture* other = NULL, * myCollider = NULL;

	if (AA != mBody && BB != mBody)
		return;

	b2Fixture* itr = mBody->GetFixtureList();
	while (itr != nullptr)
	{
		bool n = false;
		if (A == itr)
		{

			myCollider = itr;
			other = B;
			n = true;
		}
		if (B == itr)
		{
			myCollider = itr;
			other = A;
			n = true;
		}
		if (n) break;
		itr = itr->GetNext();
	}

	if (myCollider == NULL || other == NULL)
		return;


	collisionType_astroids* ct = (collisionType_astroids*)other->GetUserData();

	//if (!!ct == false) return;


	// Should the body break?
	int32 count = contact->GetManifold()->pointCount;

	float maxImpulse = 0.0f;
	for (int32 i = 0; i < count; ++i)
	{
		maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
	}
	float Density = myCollider->GetDensity();
	float Mass = mBody->GetMass();
	float DM = Density * Mass;
	// subtract from astroids health dir proportional to above comparison
	//if (*ct == collisionType_astroids::collision_astroid && maxImpulse > DM * 40.0f * physicalDefense) 
	//float  physicalDefense = 0.14f;
	if (true || maxImpulse > DM * 40.0f) //any hard collision should deal damage
	{
		float dmg = maxImpulse / (DM * 40.0f) * 100;
		attributes.UpdateCondition(-dmg * physicalDefense);
		//printf("***************************************************Ship::cpp:: dmg: %0.3f\n", dmg);
	}

}
bool Ship::handleEndContact(b2Contact* contact){
	b2Body* AA = contact->GetFixtureA()->GetBody();
	b2Body* BB = contact->GetFixtureB()->GetBody();

	if (AA != mBody && BB != mBody)
		return false;
	return true;
}
void Ship::mouseCallback(int button, int action, int mode){}


void Ship::mouseCallback_Cursor(double x, double y)
{

	if (!turn1 && !turn2) return;
	float bodyAngle = mBody->GetAngle();
	glm::vec2 mp = glm::vec2((float)x, (float)y), mpconverted;
	mpconverted = Game::AppCam->convertScreenToWorld( mp);
	target = b2Vec2(mpconverted.x, mpconverted.y);

	b2Vec2 toTarget = target - mBody->GetPosition();
	targetAngle = atan2f(-toTarget.x, toTarget.y);

	if (!turn1 && turn2) {
		mBody->SetTransform(mBody->GetPosition(), targetAngle);
	}
}

void Ship::onKeyPress(int key)
{

	bool doubleTapFrame = false;
	if (listeningForDoubleTap && lastKeyDown == key)
	{
		doubleTapEnabled = true;
		doubleTapKey = key;
		doubleTapFrame = true;
	}
	switch (key)
	{
	case GLFW_KEY_W:
	case GLFW_KEY_UP:
		thrust_forward = true;
		if (!listeningForDoubleTap) { listeningForDoubleTap = true; }
		break;
	case GLFW_KEY_S:
	case GLFW_KEY_DOWN:
		thrust_backward = true;
		if (!listeningForDoubleTap) { listeningForDoubleTap = true; }
		break;
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT:
		thrust_left = true;
		if (!listeningForDoubleTap) { listeningForDoubleTap = true; }
		break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT:
		thrust_right = true;
		if (!listeningForDoubleTap) { listeningForDoubleTap = true; }
		break;
	default:
		break;
	}


	lastKeyDown = key;

	if (doubleTapFrame)
	{
		ApplyImpulseForce();
	}
}

void Ship::onKeyRelease(int key)
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

	if (doubleTapEnabled && doubleTapKey == key) //you have released the double tapped key...
	{
		doubleTapEnabled = false;
		doubleTapKey = 0;
	}

	lastKeyUp = key;
}

void Ship::onKeyRepeat(int key)
{
}

void Ship::Draw()
{
	if (drawStats)
	{
		//DrawString(5, 0, "Body Pos: (%.3f, %.3f)", mBody->GetPosition().x, mBody->GetPosition().y);
		//DrawString(5, 12, "Body Spd: (%.3f, %.3f)", mBody->GetLinearVelocity().x, mBody->GetLinearVelocity().y);
		//DrawString(200, 12, "Body Spd: (%.3f)", mBody->GetLinearVelocity().Length());
		//DrawString(5, 25, "Target: (%3f,%3f)", target.x, target.y);
		//DrawString(5, 40, "Target Angle: %3f", targetAngle);
	}
}

void Ship::Draw(Shader* shader, Surface* surface)
{
}

#include <imgui.h>

void Ship::Debug()
{
	if (ImGui::TreeNode("basic Ship configuration"))
	{
		ImGui::SliderFloat("Defense", &physicalDefense, 0, 1.0f);
		ImGui::Checkbox("constrain velocity", &constrain_velocity);
		ImGui::SliderFloat("current Speed", &current_speed, 0, 150.0f);
		ImGui::SliderFloat("Max Speed", &maximum_speed, 0, 150.0f);
		ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0, 1.0f);
		ImGui::Checkbox("torque turn (no mouse)", &turn1);
		ImGui::Checkbox("torque turn (mouse)", &turn2);
		ImGui::Checkbox("(stats) visible", &drawStats);
		if (turnjointref)
		{
			if (ImGui::Button("Remove Joint"))
			{
				Game::m_World->DestroyJoint(turnjointref);
				turnjointref = nullptr;
			}
		}
		else
		{
			if (ImGui::Button("Add Joint"))
			{
				turnjointref = attachFrictionJoint2(groundBodyRef, mBody);
			}
		}
		attributes.Debug("< Attributes >");
		if (ImGui::TreeNode("Thrust Debug"))
		{
			ImGui::Checkbox("forward thrust", &thrust_forward);
			ImGui::Checkbox("backward thrust", &thrust_backward);
			//ImGui::Checkbox("left thrust", &thrust_left);
			//ImGui::Checkbox("right thrust", &thrust_right);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

