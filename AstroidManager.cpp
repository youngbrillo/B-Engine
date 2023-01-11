#include "AstroidManager.h"
using namespace SpaceBuster;

#include "ResourceManager.h"
#include "defaults_SpaceBuster.h"
#include "ItemObject.h"
#include "ObjectFactory.h"


SpaceBuster::AstroidFactory::AstroidFactory(const unsigned int limit, const b2Vec2& lowerBound, const b2Vec2& upperBound)
	: max(limit), count(0)
	, SpawnTimeTarget(5.0f)
	, timeLeftUntilNextSpawn(5.0f)
	, spawnLowerBound(lowerBound)
	, spawnUpperBound(upperBound)
	, autoGenerateEnabled(true)
	, spawnAngle(0.0f)
	, maxAstroidSize(20.0f)
	, groundRef(nullptr)
	, amountDeleted(0)
	, lastFrameDelete(false)
	, lastDeletedPos(b2Vec2_zero), lastDeletedSpeed(b2Vec2_zero)
	, baseChance(0.5f)
{
	//Astroids.clear();

	def_template.color = glm::vec4(1, 1, 0, 1);
	def_template.texture = ResourceManager::GetTexturePtr("icon");
	def_template.enabled_physics = true;
	def_template.name = "Astroid";
}

SpaceBuster::AstroidFactory::~AstroidFactory()
{
	groundRef = nullptr;
	listToCreate.clear();
}


void SpaceBuster::AstroidFactory::Update(float dt)
{
	if (lastFrameDelete)
	{
		if (listToCreate.size() > 0)
		{
			for (int i = 0; i < listToCreate.size(); i++)
			{
				this->createAstroid(listToCreate[i].position, listToCreate[i].linearVelocity, listToCreate[i].angularVelocity, b2Vec2(listToCreate[i].size.x / 2, listToCreate[i].size.y / 2), true);
				this->createAstroid(listToCreate[i].position, -listToCreate[i].linearVelocity, -listToCreate[i].angularVelocity, b2Vec2(listToCreate[i].size.x / 2, listToCreate[i].size.y / 2), true);

			}


			listToCreate.clear();
		}
		else
		{
			//there is a chance for some good loot to drop here
			float chance = 0.5f;

			time_t seed = time(NULL); //current time

			std::srand(seed); //generate the current seed;

			chance = ((rand() % 101) / 100.0f);
			printf("What is the chance for some loot to drop? %0.2f/%0.2f\n", chance, baseChance);

			if (chance >= baseChance)
			{
				baseChance += 0.05f;
				SpaceBuster::ItemType currentItem = SpaceBuster::ItemType::EnergyCell;
				const int size = 2;//4;
				SpaceBuster::ItemType dropTypes[size] = {
					SpaceBuster::ItemType::SparePart
					,SpaceBuster::ItemType::EnergyCell
					//,SpaceBuster::ItemType::Ammo_reg
					//,SpaceBuster::ItemType::Ammo_Reflect
				};

				int currIndex = rand() % size;

				currentItem = dropTypes[currIndex];

				//GameObjectDefinition gd = DefaultItemDefinition(lastDeletedPos, lastDeletedPos, currentItem );

				//ObjectFactory::InsertObject(new ItemObject(ResourceManager::GetItemPtr(currentItem), &gd));

				GameObjectDefinition gd;
				gd.position = glm::vec3(lastDeletedPos.x, lastDeletedPos.y, 0.0f);
				gd.enabled_physics = true;
				gd.scale = 3.0f;


				b2BodyDef bd;
				bd.type = b2BodyType::b2_dynamicBody;
				bd.position = b2Vec2(lastDeletedPos.x, lastDeletedPos.y);
				bd.linearVelocity = lastDeletedSpeed;
				bd.fixedRotation = true;

				gd.objBodydef = &bd;

				b2FixtureDef fd;
				fd.isSensor = true;
				//b2CircleShape shape;
				//shape.m_radius = 1.0f;
				b2PolygonShape shape;
				shape.SetAsBox(gd.scale * gd.size.x / 2, gd.scale * gd.size.y / 2);

				fd.shape = &shape;

				gd.objFixDef = &fd;
				gd.texture = ResourceManager::GetTexturePtr(ResourceManager::lookupItemToTexture[currentItem]);

				GameObject* obj = new ItemObject(ResourceManager::GetItemPtr(currentItem), &gd);

				ObjectFactory::InsertObject(obj);
			}
			else
			{
				baseChance -= 0.02f;
			}
		}
		lastFrameDelete = false;
	}
	if (autoGenerateEnabled && count < max)
	{
		autoGenerate(dt);
	}

}

#include<random>

struct AstroidParams
{
	b2Vec2 s_position, s_linearVel, s_size;
	float angle;
};


void SpaceBuster::AstroidFactory::autoGenerate(float dt)
{
	timeLeftUntilNextSpawn -= dt;
	if (timeLeftUntilNextSpawn > 0.0f) return;

	float dir1, dir2;
	int m_size = (int)maxAstroidSize;

	time_t seed = time(NULL); //current time

	std::srand(seed); //generate the current seed;

	AstroidParams sp; //starting parameters
	dir1 = rand() % 2 == 0 ? 1.0f : -1.0f;
	dir2 = rand() % 2 == 0 ? 1.0f : -1.0f;


	sp.s_position = b2Vec2(dir1 * ((rand() % 101) / 100.0f) * spawnUpperBound.x, dir2 * ((rand() % 101) / 100.0f) * spawnUpperBound.y);

	dir1 = rand() % 2 == 0 ? 1.0f : -1.0f;
	dir2 = rand() % 2 == 0 ? 1.0f : -1.0f;

	sp.s_linearVel = b2Vec2(dir1 * ((rand() % 101) / 100.0f) * spawnLowerBound.x, dir2 * ((rand() % 101) / 100.0f) * spawnLowerBound.y);
	float r_s = (rand() % m_size) + 1;
	sp.s_size = b2Vec2(r_s, r_s);
	//sp.s_size = b2Vec2((rand() % m_size) + 1, (rand() % m_size) + 1);

	sp.angle = dir1 * (rand() % 361);



	createAstroid(sp.s_position, sp.s_linearVel, 0.0f, sp.s_size);

	timeLeftUntilNextSpawn = SpawnTimeTarget;
}

void SpaceBuster::AstroidFactory::createAstroid(const b2Vec2& s, const b2Vec2& lv, const float& av, b2Vec2 _size, bool overrideCountRestriction)
{
	if (count >= max && !overrideCountRestriction)
	{
		//printf("Cannot create more of type: %s, maximum threshold (%d) has been reached\n", "ASTROID", max);
		return;
	}


	b2PolygonShape shape;
	if (_size.x < 1.0f) {
		_size.x = 1.0f; _size.y = 1.0f;
	}
	shape.SetAsBox(_size.x / 2.0f, _size.y / 2.0f);


	//printf("NEW astroid size (%.2f, %.2f)\n", _size.x / 2.0f, _size.y / 2.0f);
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;
	fd.friction = 0.3f;
	fd.restitution = .40f;

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(s.x, s.y);
	bd.linearVelocity = lv;
	bd.angularVelocity = av;

	def_template.objBodydef = &bd;
	def_template.objFixDef = &fd;
	def_template.size = glm::vec3(_size.x, _size.y, 1);

	AstroidObject* new_astroid = new AstroidObject(&def_template, this);
	if (groundRef)
		attachFrictionJoint(groundRef, new_astroid->GetBody());

	ObjectFactory::InsertObject(new_astroid);
	//Astroids.emplace_back(new_astroid);

	count++;
}

void SpaceBuster::AstroidFactory::handleDelete(AstroidObject* i)
{
	count--;
	amountDeleted++;

	lastFrameDelete = true;
	lastDeletedPos = i->GetBody()->GetPosition();
	//lastDeletedSpeed = i->GetBody()->GetLinearVelocity();
}

void SpaceBuster::AstroidFactory::handleFragment(AstroidState& state, AstroidObject* i)
{

	if (!i->fragmentFlag) {
		printf("AstroidManager.cpp::\tHow did this section of code even get reached?!\n");
		return;
	}
	//this->createAstroid(state.position, state.linearVelocity, state.angularVelocity, b2Vec2(i->size.x / 2, i->size.y / 2), true);
	//this->createAstroid(state.position, -state.linearVelocity, -state.angularVelocity, b2Vec2(i->size.x/2, i->size.y/2), true);
	
	//i->deletionFlag = true;
	i->broken = true;
	listToCreate.push_back(state);

}


// DEBUGGIN
#include "imgui.h"
void SpaceBuster::AstroidFactory::Debug()
{
	if (ImGui::TreeNode("-Astroid Factory-"))
	{
		ImGui::Checkbox("Auto Spawn", &autoGenerateEnabled);
		ImGui::SliderInt("astroids destroyed", &amountDeleted, 0, 0);
		ImGui::SliderFloat("chance to spawn item", &baseChance, 0, 0);
		ImGui::SliderInt("max astroid spawnable", &max, 1, 50);


		if (ImGui::TreeNode("-Spawning-"))
		{
			ImGui::SliderFloat("Spawn time left", &timeLeftUntilNextSpawn, 0, SpawnTimeTarget);
			ImGui::SliderFloat("Spawn time Limit", &SpawnTimeTarget, 0, 10.0f);

			ImGui::SliderFloat2("rand pos", &spawnUpperBound.x, 0, 50);
			ImGui::SliderFloat2("rand vel", &spawnLowerBound.x, 0, 50);
			ImGui::SliderFloat("rand Ang", &spawnAngle, -50, 50);

			ImGui::TreePop();

		}
		ImGui::TreePop();
	}
}
