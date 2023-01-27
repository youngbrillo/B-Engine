#include "OmniLauncher.h"
#include "Game.h"
#include <imgui.h>
/*
	Projectile
*/
const float PROJECTILE_LIFESPAN = 5.0f;
OmniProjectile::OmniProjectile(const ProjectileType& projectileType, const b2Vec2& launchPosition, const b2Vec2& launchVelocity, int assignedLevel)
	: body (nullptr)
	, isDestroyed (false)
	, hasMadeContact (false)
	, lifeRemaining (0.0f)
	, lifeSpan(PROJECTILE_LIFESPAN + (level - 1))
	, collisionType (collisionType_astroids::collision_bullet)
	, type (projectileType)
	, level(assignedLevel)
{

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position = launchPosition;
	bd.bullet = true;
	body = Game::m_World->CreateBody(&bd);
	body->SetLinearVelocity(launchVelocity);

	b2CircleShape circle;
	//b2PolygonShape rect;

	circle.m_radius = 0.3f;
	//rect.SetAsBox(10, 0.3f);

	b2FixtureDef fd;
	fd.shape = &circle;
	fd.density = type == ProjectileType::projectile_standard ? 40.0f * level : 40.0f; //inc bullet density so that astroid destruc can happen sooner (from 20)
	fd.restitution = 1.00f;

	auto* fix = body->CreateFixture(&fd);

	fix->SetUserData(&this->collisionType);
}

OmniProjectile::~OmniProjectile()
{
	Game::m_World->DestroyBody(body);
	body = nullptr;
}

void OmniProjectile::Update(float dt)
{
	if (isDestroyed) return;

	lifeRemaining += dt;
	if (lifeRemaining >= lifeSpan)
	{
		isDestroyed = true;
	}

	this->handleContact();
}

void OmniProjectile::handleContact()
{
	if (!hasMadeContact) return;

	isDestroyed = true;
}

void OmniProjectile::setContact(bool madeContact)
{
	switch (type)
	{
	case projectile_bounce:
		if (level == 0) hasMadeContact = true;
		else {
			level--;
			lifeSpan -= PROJECTILE_LIFESPAN * 0.2;
		}
		break;

	case projectile_homing:
		printf("Add a target vector and a homing function (Read that AI/State machine book)\n");
		hasMadeContact = true;
		break;
	//case projectile_fragment:
	case projectile_standard:
	default:
		hasMadeContact = true;
		break;
		break;
	}
}

void OmniProjectile::Draw(Shader* shader, Surface* surface)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	Launcher	Launcher	Launcher	Launcher	Launcher	Launcher	Launcher	Launcher	Launcher	Launcher	Launcher
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * *  * * */



OmniLauncher::OmniLauncher(float starting_rate_of_fire, int startingLevel)
	: rate_of_fire(starting_rate_of_fire)
	, rate_of_fire_time_left(0.0f)
	, canShoot(true)
	, modesIndex(0), ltIndex(0)
	, mode(ProjectileType::projectile_standard)
	, modeAlt(LaunhcerType::launch_normal)
	, level(startingLevel)
	, SpreadAngle(11.310f * b2_pi/180.0f)
{
}

OmniLauncher::OmniLauncher(OmniLauncherDefinition* definition)
	: rate_of_fire(definition->rate_of_fire)
	, mode(definition->AmmoType)
	, modeAlt(definition->FireMode)
	, level(definition->saved_level)
	, modesIndex(0), ltIndex(0)
	, rate_of_fire_time_left(0.0f)
	, SpreadAngle(11.310f * b2_pi / 180.0f)
	, canShoot(true)
{
}

OmniLauncher::~OmniLauncher()
{
	auto iter = projectiles.begin();
	while (iter != projectiles.end())
	{
		auto obj = (*iter);
		iter = projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), obj), projectiles.end());
		delete obj;
		obj = nullptr;
	}
	projectiles.clear();
}

void OmniLauncher::deleteFlagedProjectiles()
{

	auto iter = projectiles.begin();
	while (iter != projectiles.end())
	{
		auto obj = (*iter);
		if (obj->isDestroyed)
		{
			iter = projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), obj), projectiles.end());

			delete obj;
			obj = nullptr;
		}
		else {
			iter++;
		}
	}

}


void OmniLauncher::Update(float dt)
{
	
	deleteFlagedProjectiles();

	for (auto i : projectiles) i->Update(dt);

	if (!canShoot)
	{
		rate_of_fire_time_left += dt;
		if (rate_of_fire_time_left >= rate_of_fire)
		{
			canShoot = true;
			rate_of_fire_time_left = 0.0f;
		}


	}

}


void OmniLauncher::Shoot(const b2Vec2& position, const b2Vec2& velocity, const float& angle_rad)
{
	if (!canShoot) return;
	float angle_between = 0.0f;
	switch (modeAlt)
	{
	case launch_Spread:
		for (int i = -level; i <= level; i++)
		{
			angle_between = SpreadAngle * i;
			//to rotate a vector around an angle alpha multiply it by the following matrix
			/*
				[  cos(alpha)	sin(alpha) ]
				[ -sin(alpha)	cos(alpha) ]
			*/
			b2Mat22 mat = b2Mat22(cos(angle_between), sin(angle_between), -sin(angle_between), cos(angle_between));
			b2Vec2 modVel = b2MulT(mat, velocity);
			projectiles.emplace_back(new OmniProjectile(mode, position, modVel, level));

		}
		break;
	case launch_directional:
		for (int i = 0; i < 4; i++)
		{
			angle_between = 90 * b2_pi/180.0f * i;
			b2Mat22 mat = b2Mat22(cos(angle_between), sin(angle_between), -sin(angle_between), cos(angle_between));
			b2Vec2 modVel = b2MulT(mat, velocity);
			projectiles.emplace_back(new OmniProjectile(mode, position, modVel, level));
		}
		break;
	case launch_omni:
		for (int i = 0; i < 8; i++)
		{
			angle_between = 45 * b2_pi / 180.0f * i;
			b2Mat22 mat = b2Mat22(cos(angle_between), sin(angle_between), -sin(angle_between), cos(angle_between));
			b2Vec2 modVel = b2MulT(mat, velocity);
			projectiles.emplace_back(new OmniProjectile(mode, position, modVel, level));
		}
		break;
	case launch_normal:
	default:
		projectiles.emplace_back(new OmniProjectile(mode, position, velocity, level));
		break;
	}
	 

	canShoot = false;
	rate_of_fire_time_left = 0.0f;

}


void OmniLauncher::handleCollision(b2Contact* contact)
{
	b2Body* A = contact->GetFixtureA()->GetBody();
	b2Body* B = contact->GetFixtureB()->GetBody();

	for (auto i : projectiles)
	{
		if (i) {

			if (i->body == A || i->body == B)
			{
				b2Fixture* projFixture, * otherFixture;
				if (i->body == A) 
				{ 
					projFixture = contact->GetFixtureA();
					otherFixture = contact->GetFixtureB();
				}
				else 
				{ 
					projFixture = contact->GetFixtureB();
					otherFixture = contact->GetFixtureA();
				}

				collisionType_astroids* k = (collisionType_astroids*)otherFixture->GetUserData();
				if (!!k)
				{	//make bullets ignore the ship
					if (*k == collisionType_astroids::collision_ship || *k == collisionType_astroids::collision_bullet) {
						contact->SetEnabled(false);
						break;
					}
				}  
				i->setContact(true);
			}
		}
	}
}



const  ProjectileType projectileFilter[3] = { projectile_standard, projectile_bounce, /*projectile_fragment,*/ projectile_homing };
const char* projectilefileterNames[3] = { "Standard", "Reflect", "Missle"};

const  LaunhcerType launchertypes[4] = { launch_normal, launch_Spread, launch_directional /*, launch_Diagonal*/, launch_omni};
const  char*  launchertypenames[4] = { "Forward", "Spread", "Rook-wise", "Omni-directional"};


void OmniLauncher::ChangeMode(const ProjectileType& projectiletype)
{
	mode = projectiletype;
	for (int i = 0; i < sizeof(projectileFilter) / sizeof(projectileFilter[0]); i++)
	{
		if (mode == projectileFilter[i]) {
			modesIndex = i;
			break;
		}
	}

}

void OmniLauncher::ToggleAmmoType()
{
	switch (mode)
	{
	case projectile_standard:
		ChangeMode(projectile_bounce);
		break;
	case projectile_bounce:
		ChangeMode(projectile_homing);
		break;
	case projectile_homing:
		ChangeMode(projectile_standard);
		break;
	default:
		break;
	}
}

void OmniLauncher::ToggleFireMode()
{
	switch (modeAlt)
	{
	case launch_normal:
		ChangeFireMode(launch_Spread);
		break;
	case launch_Spread:
		ChangeFireMode(launch_directional);
		break;
	case launch_directional:
		ChangeFireMode(launch_omni);
		break;
	case launch_omni:
		ChangeFireMode(launch_normal);
		break;
	default:
		break;
	}
}

void OmniLauncher::ChangeFireMode(const LaunhcerType& fireMode)
{
	modeAlt = fireMode;
	for (int i = 0; i < sizeof(launchertypes) / sizeof(launchertypes[0]); i++)
	{
		if (modeAlt == launchertypes[i]) {
			ltIndex = i;
			break;
		}
	}
}

void OmniLauncher::Debug()
{


	if (ImGui::TreeNode("- Omni Launcher -"))
	{
		ImGui::Checkbox("can shoot", &canShoot);
		ImGui::SliderAngle("Launch Angle", &SpreadAngle);

		if (ImGui::SliderInt("projectile type", &modesIndex, 0, sizeof(projectileFilter) / sizeof(projectileFilter[0]) -1, projectilefileterNames[modesIndex]))
		{
			mode = projectileFilter[modesIndex];
		}

		if (ImGui::SliderInt("Fire mode", &ltIndex, 0, sizeof(launchertypes) / sizeof(launchertypes[0]) -1, launchertypenames[ltIndex]))
		{
			modeAlt = launchertypes[ltIndex];
		}
		ImGui::SliderInt("level", &level, 1, 10);
		ImGui::SliderFloat("Rof time left", &rate_of_fire_time_left, 0, 0);
		ImGui::SliderFloat("Rate of fire", &rate_of_fire, 0, 1.0f);
		ImGui::TreePop();
	}
}
