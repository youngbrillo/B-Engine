#pragma once
#include <Box2D/Box2D.h>

struct ShipAttributeDef
{
	b2Vec2	ThrustForce = b2Vec2(0.0f, -50.0f)
		,	ThrustPosition = b2Vec2(0.0f, 3.0f);

	float TorqueMag = 10.0f
		, startingCondition = 100.0f
		, startingEnergy = 100.0f
		, maxCondition = 100.0f
		, maxEnergy = 100.0f
		, physicalDefense = 0.14f
		, energyCost = 0.33f
		, torqueRatio = 0.1f
		, impulseCostModifier = 2.0f;

};

class ShipAttributes
{
public:
	ShipAttributes() {};
	ShipAttributes(ShipAttributeDef* def);
	ShipAttributes(const char* attributeFile);
	~ShipAttributes();

	void UpdateCondition(float difference);
	void UpdateEnergy(float difference);
	float getCondition() { return currentCondition; }
	float getEnergy() { return currentEnergy; }
	void LoadAttributeFile(const char* fileName);
	void Debug(const char* name = "ShipAttributes");
private:
	b2Vec2 ThrustForce, ThrustPosition;

	float TorqueMag
		, startingCondition, maxCondition, currentCondition
		, startingEnergy, maxEnergy, currentEnergy
		, physicalDefense
		, energyCost, torqueRatio, impulseCostModifier;

	bool operational; //is there enough energy to power the ship
	bool broken; // is the ship completely broken

	friend class Ship;
	friend class ShipControllerUI;
	friend class NeoInventory;
};

