#pragma once
#include "GameObject.h"
#include "Item.h"

class ItemObject : public GameObject
{
public: 
	ItemObject(SpaceBuster::Item* i, GameObjectDefinition* def, float value = 10, int amount = 1);
	~ItemObject();


	//virtual void Draw(Surface* renderSurface = nullptr)
	virtual void handleBeginContact(b2Contact* contact)override;
	virtual void handleEndContact(b2Contact* contact)override;

	virtual void Debug() override;
private:
	SpaceBuster::Item* itemReference;
	float itemValue;
	int amount;
	b2Fixture* sensorFixture;

	friend class Inventory;
};

