#include "Bullet.h"
#include "Game.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"



//projectiles >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

SpaceBuster::Bullet::Bullet(const b2Vec2& launchPosition, const b2Vec2& launchVelocity)
	: destroy(false), madeContact(false)
	, lifeRemaining(0.0f), lifeMax(3.0f)
	, collisionType(collisionType_astroids::collision_bullet)
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
	fd.density = 40.0f; //inc bullet density so that astroid destruc can happen sooner (from 20)
	fd.restitution = 1.00f;

	b2Vec2 minV = launchPosition - b2Vec2(0.3f, 0.3f);
	b2Vec2 maxV = launchPosition + b2Vec2(0.3f, 0.3f);

	b2AABB aabb;
	aabb.lowerBound = minV;
	aabb.upperBound = maxV;

	auto* fix = body->CreateFixture(&fd);

	fix->SetUserData(&this->collisionType);
}

SpaceBuster::Bullet::~Bullet()
{
	Game::m_World->DestroyBody(body);
	body = NULL;
}

void SpaceBuster::Bullet::Update(float dt)
{
	lifeRemaining += dt;
	if (lifeRemaining >= lifeMax)
	{
		destroy = true;
		lifeRemaining = 0.0f;
	}

	if (madeContact) { destroy = true; }
}




//projectile launchers >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//todo:: make auto launcher implement pooling!!!
SpaceBuster::AutoLauncher::AutoLauncher(int max)
	: SpaceBuster::Launcher()
	, magCurrent(max)
	, magCapacity(max)
	, rateCurrent(0)
	, rateofFire(0.1f)
	, canShoot(true)
	, poolsize(0)
{
	bullets.clear();
}

SpaceBuster::AutoLauncher::~AutoLauncher()
{
	bullets.clear();
}

void SpaceBuster::AutoLauncher::Update(float dt)
{
	std::vector<Bullet*> to_delete;
	for (auto i : bullets)
	{
		i->Update(dt);
		if(i->canDestroy())
			to_delete.emplace_back(i);
	}



	for (auto i : to_delete)
	{
		bullets.erase(std::remove(bullets.begin(), bullets.end(), i), bullets.end());
		delete i;
		i = NULL;
	}

	to_delete.clear();

	if (canShoot != true)
	{
		rateCurrent += dt;
		if (rateCurrent >= rateofFire)
		{
			canShoot = true;
			rateCurrent = 0.0f;
		}

	}

	poolsize = bullets.size();
}

void SpaceBuster::AutoLauncher::Shoot(const b2Vec2& position, const b2Vec2& velocity)
{
	if (autoReload && magCurrent == 0) {
		Reload();
	}
	if (!enabled) return;
	if (!canShoot) return;
	if (magCurrent == 0) return;

	--magCurrent;
	bullets.emplace_back(new Bullet(position, velocity));
	canShoot = false;
	rateCurrent = 0.0f;
}

void SpaceBuster::AutoLauncher::Reload()
{
	magCurrent = magCapacity;
}

void SpaceBuster::AutoLauncher::handleCollision(b2Contact* contact)
{
	b2Body* A = contact->GetFixtureA()->GetBody();
	b2Body* B = contact->GetFixtureB()->GetBody();
	
	for (auto i : bullets)
	{
		if (i) {
			//if (i->madeContact)
			//{
			//	contact->SetEnabled(false);
			//	continue;
			//}
			if (i->getBody() == A || i->getBody() == B)
			{
				//make bullets ignore the ship
				b2Fixture* a,  *b;
				if (i->getBody() == A) { a = contact->GetFixtureA(); b = contact->GetFixtureB(); }
				else{ a = contact->GetFixtureB();  b = contact->GetFixtureA();}

				collisionType_astroids* k = (collisionType_astroids*)b->GetUserData();
				if (!!k)
				{
					if (*k == collisionType_astroids::collision_ship) {
						contact->SetEnabled(false);
						break;
					}
				}
				
				i->setContact(true);
			}
		}
	}
}

void SpaceBuster::AutoLauncher::Debug()
{
	if (ImGui::TreeNode("- Auto Launcher -"))
	{
		ImGui::Checkbox("Auto Reload", &autoReload);
		ImGui::SliderFloat("Rof time left", &rateCurrent, 0, 0);
		ImGui::SliderFloat("Rate of fire", &rateofFire, 0, 1.0f);
		ImGui::SliderInt("rounds in mag", &magCurrent, 0, magCapacity);
		ImGui::SliderInt("mag Capacity", &magCapacity, 0, 300);
		ImGui::SliderInt("VectorSize", &poolsize, 0, 0);

		ImGui::TreePop();
	}
}

void SpaceBuster::Launcher::ShootBounce(const b2Vec2& position, const b2Vec2& velocity)
{
	if (!enabled) return;
	if (m_projectile)
	{
		return;
	}
	m_projectile = new BouncingBullet(position, velocity);
}

void SpaceBuster::AutoLauncher::ShootBounce(const b2Vec2& position, const b2Vec2& velocity)
{
	if (autoReload && magCurrent == 0) {
		Reload();
	}
	if (!enabled) return;
	if (!canShoot) return;
	if (magCurrent == 0) return;

	--magCurrent;
	bullets.emplace_back(new BouncingBullet(position, velocity));
	canShoot = false;
	rateCurrent = 0.0f;
}
