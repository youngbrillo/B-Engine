#include "ItemObject.h"
#include "Utility_Physics.h"
#include "ShipController.h"

using namespace SpaceBuster;
ItemObject::ItemObject(Item* i, GameObjectDefinition* def, float value, int amnt)
	: GameObject(def)
	, itemReference(i)
	, itemValue(value)
	, amount(amnt)
{
	name = itemReference->GetName();

	b2FixtureDef fd;
	fd.isSensor = true;
	b2CircleShape shape;
	shape.m_radius = def->size.x * def->scale * 2.0f;
	fd.shape = &shape;

	sensorFixture = m_body->CreateFixture(&fd);
}


ItemObject::~ItemObject()
{
	itemReference = nullptr;
}

void ItemObject::handleBeginContact(b2Contact* contact)
{
	if (!m_contactFixture || !enabled_physics) return;
	//if the other is of type player, then add the item to their inventory
	b2Fixture* A = contact->GetFixtureA();
	b2Fixture* B = contact->GetFixtureB();
	b2Fixture* other;
	if (A == sensorFixture)  other = B;
	else if (B == sensorFixture) other = A;
	else  return; //not the item wrapper...

	collisionType_astroids* k = (collisionType_astroids*)other->GetUserData();

	if (!!k)
	{
		if (*k == collisionType_astroids::collision_ship)
		{
			//use the said item
			//the auto use functionality works like this but i want it to be up to the ship whether or not auto use should take effect
			//printf("Collided w/ shipt\n");
			ShipController* controller = (ShipController*)other->GetBody()->GetUserData();
			if (controller)
			{
				if (controller->AddToInventory(this->itemReference))
					deletionFlag = true;
				else
					printf("ItemObject.cpp::\tFAILURE to Add %s to Ship controller inventory\n", itemReference->GetName().c_str());
			}
			else
			{
				printf("ItemObject.cpp::\tFAILURE to Add %s to inventory : could not find controller!\n", itemReference->GetName().c_str());
			}


			//if (itemReference->autoUse) {
			//	itemReference->Use(itemValue);
			//}
			//else {
			//	//get inventory, and increase instance/add amount of item
			//}

			//this->deletionFlag = true;
		}
	}


}

void ItemObject::handleEndContact(b2Contact* contact)
{
}

void ItemObject::Debug()
{
	GameObject::Debug();
}
