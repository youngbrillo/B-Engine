#pragma once
//#include "Bullet.h"
#include <vector>
#include <Box2D/Box2D.h>
#include "Utility_Physics.h"
#include "Shader.h"
#include "Surface.h"

enum ProjectileType 
{
	projectile_standard = 0,
	projectile_bounce = 1,
	//projectile_fragment = 2,
	projectile_homing = 3
};

enum LaunhcerType
{
	launch_normal = 0,
	launch_Spread = 1,
	launch_directional = 2,
	//launch_Diagonal = 3,
	launch_omni = 4
};
class OmniProjectile
{
public:
	OmniProjectile(const ProjectileType& projectileType, const b2Vec2& launchPosition, const b2Vec2& launchVelocity, int assignedLevel = 1);
	~OmniProjectile();
	
	void Update(float dt);
	void setContact(bool madeContact);
	void handleContact();
	void Draw(Shader* shader, Surface* surface);
	
public:
	b2Body* body;
	bool isDestroyed, hasMadeContact;
	float lifeRemaining, lifeSpan;
	
	collisionType_astroids collisionType;
	ProjectileType type;
	int level;

};

//infinite projectiles
//only limit is firerate
//toggle between fire modes and projectile types
struct OmniLauncherDefinition
{
	float rate_of_fire = 0.1f;
	int saved_level = 1;

	ProjectileType AmmoType = ProjectileType::projectile_standard;
	LaunhcerType FireMode = LaunhcerType::launch_normal;
};
class OmniLauncher
{
public:
	OmniLauncher(float starting_rate_of_fire = 0.1f, int startingLevel = 1);
	OmniLauncher(OmniLauncherDefinition* definition);
	~OmniLauncher();
	//deletes projectiles whose 'isDestroyed' flag is set to true
	void deleteFlagedProjectiles();
	void Update(float dt);
	void Shoot(const b2Vec2& position, const b2Vec2& velocity, const float& angle);
	void ChangeMode(const ProjectileType& mode);
	void ToggleAmmoType();
	void ToggleFireMode();
	void ChangeFireMode(const LaunhcerType& fireMode);
	void handleCollision(b2Contact* contact);
	void Debug();
private:
	std::vector<OmniProjectile*> projectiles;
	float rate_of_fire, rate_of_fire_time_left;
	bool canShoot;

	ProjectileType mode;
	LaunhcerType modeAlt;
private: //debug
	int modesIndex, ltIndex, level;
	float SpreadAngle;

	friend class NeoShipController;
};
