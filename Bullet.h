#pragma once
#ifndef BULLET_H
#define BULLET_H

#include <Box2D/Box2D.h>
#include <vector>
#include "Utility_Physics.h"


namespace SpaceBuster {

	class Bullet
	{
	public:
		Bullet(const b2Vec2& launchPosition, const b2Vec2& launchVelocity);
		virtual ~Bullet();
			
		virtual void Update(float dt);
		virtual void Draw() {}; //not implemented yet, will do next to last

		virtual void setContact(bool v) { madeContact = v; }
		bool getContact() { return madeContact; }
		bool canDestroy() { return destroy; }
		b2Body* getBody() { return body; }
	protected:
		b2Body* body;
		bool destroy, madeContact;
		float lifeRemaining,lifeMax;
		
		friend class Launcher;
		friend class AutoLauncher;

		collisionType_astroids collisionType;
	};

	class BouncingBullet : public Bullet
	{
	public:
		BouncingBullet(const b2Vec2& launchPosition, const b2Vec2& launchVelocity) : Bullet(launchPosition, launchVelocity), hits(0), hitMax(1) {};
		virtual ~BouncingBullet() {}
		virtual void setContact(bool v)  override
		{ 
			madeContact = v; 
			if (v) { 
				madeContact = hits <= hitMax;
				hits++;
				if(hits <= hitMax)
					lifeRemaining = 0.0f;
			} 
		}
		virtual void Update(float dt) override
		{
			lifeRemaining += dt;
			if (lifeRemaining >= lifeMax)
			{
				destroy = true;
				lifeRemaining = 0.0f;
			}
			if (hits > hitMax){
				destroy = true;
			}
		}

	protected:
		int hits, hitMax;
	};


	class Launcher
	{
	public:
		Launcher() : enabled(true), autoReload(true)
			, m_projectile(NULL)
			, projectileStartPos(0.0f, -0.5f)
			, projectileVelocity(0.0f, -50.0f)
			, ammoCountPrimary(100), ammoCountSecondary(10)
		{

		};
		virtual ~Launcher(){
			if (m_projectile)
			{
				if (m_projectile->destroy)
				{
					delete m_projectile;
					m_projectile = NULL;
				}
			}
		};

		virtual void Update(float dt)
		{
			if(m_projectile)
				m_projectile->Update(dt);


			if (m_projectile)
			{
				if (m_projectile->destroy) 
				{
					delete m_projectile;
					m_projectile = NULL;
				}
			}
		}

		virtual void Shoot(const b2Vec2& position, const b2Vec2& velocity)
		{
			if (!enabled) return;
			if (m_projectile)
			{
				return;
			}
			m_projectile = new Bullet(position, velocity);
		}
		virtual void ShootBounce(const b2Vec2& position, const b2Vec2& velocity);
		bool enabled, autoReload;
		Bullet* m_projectile;
		b2Vec2 projectileStartPos, projectileVelocity;
		int ammoCountPrimary, ammoCountSecondary;

		virtual void Reload() {};
		virtual void handleCollision(b2Contact* contact) 
		{
			if (this->m_projectile) return;
			b2Body* A = contact->GetFixtureA()->GetBody();
			b2Body* B = contact->GetFixtureB()->GetBody();

			if (A != this->m_projectile->getBody() && B != this->m_projectile->getBody()) return;

			//bomb has collided with something

			this->m_projectile->setContact(true);
		}

		virtual void Debug() {};

	};



	class  AutoLauncher : public Launcher
	{
	public:
		AutoLauncher(int max = 5);
		~AutoLauncher();

		virtual void Debug() override;

		virtual void Update(float dt) override;
		virtual void Shoot(const b2Vec2& position, const b2Vec2& velocity) override;
		virtual void ShootBounce(const b2Vec2& position, const b2Vec2& velocity) override;
		virtual void Reload() override;
		virtual void handleCollision(b2Contact* contact) override;
		std::vector<Bullet*> bullets;

		int magCurrent, magCapacity, poolsize;
		float rateCurrent, rateofFire;
		bool canShoot; //so as to not exceed the rate of fire

	};
}

#endif // !BULLET_H
