#pragma once
#ifndef ITEM_H
#define ITEM_H

#include <Box2D/Box2D.h>
#include <iostream>

namespace SpaceBuster {
	enum class ItemType
	{
		null_item	= 0, //aka idk what this item type is, for error catching
		SparePart	= 1,
		EnergyCell	= 2,
		Ammo_reg	= 3,
		Ammo_Reflect= 4,
		//upgrades
		Upgrade_speed = 5,
		Upgrade_health = 6,
		Upgrade_fireRate = 7,
		Upgrade_defense = 8,
		Upgrade_energyReduction = 9,
		Upgrade_energyTorqueReduction = 10,
		//tools
		tool_winch	= 11, //tow astroids, maybe sling shot them
		tool_forcefield = 12, // expell force in a set radius from center of ship
		tool_gravityBomb = 13, //same as zone just compact and w/ unlimited capacity, has short timer so will destruct after capacity has been reached or timer goes off.
		currency = 14,
	};

	class Item //need to make a dictionary class that holds a table of references to each unique item
	{
	public:
		Item();
		Item(ItemType itemType, std::string itemname, std::string itemDesc, bool canUseAutomatically = false, bool isStackable = true);
		~Item();

		virtual bool Use(float value = 1.0f);//, Spaceship* user = nullptr);

		const ItemType GetType() { return type; };
		const std::string GetName() { return name; }
		const std::string GetDescription() { return description; }
		bool autoUse;
		bool canStack;
	protected:
		ItemType type;
		std::string name, description;

	public:

	private:
		friend class Inventory;
	};

}
#endif // !ITEM_H