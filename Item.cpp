#include "Item.h"
#include "Game.h"

//set globals

#include "ShipController.h"


SpaceBuster::Item::Item()
	: type(ItemType::null_item)
	, name("Unknown Item")
	, description("It looks like you used the wrong constructor to create an item...")
	, autoUse(false)
	, canStack(false)
{
}

SpaceBuster::Item::Item(ItemType itemType, std::string itemname, std::string itemDesc, bool canUseAutomatically, bool isStackable)
	: type(itemType)
	, name(itemname)
	, description(itemDesc)
	, autoUse(canUseAutomatically)
	, canStack(isStackable)
{
	//printf("Creating Item %s\n", name.c_str());
}

SpaceBuster::Item::~Item()
{
	//printf("Deleting Item %s\t-bye bye!\n", name.c_str());d
}

bool SpaceBuster::Item::Use(float value)//, Spaceship* user)
{
	//first get the user
	Spaceship* user = nullptr;
	bool canContinue = user != nullptr;

	
	if (!canContinue && ShipController::g_shipController) {

		user = ShipController::g_shipController->m_ship;
		canContinue = user != nullptr;
	}

	if (!canContinue)
	{
		printf("Item::cpp::\tNo user has been assigned!!!!\n");
		return false;
	}


	switch (type)
	{
		case SpaceBuster::ItemType::SparePart: //restore condition
			user->modifyCondition(value);
			break;
		case SpaceBuster::ItemType::EnergyCell: //restore energy
			user->modifyEnergy(value);
			break;
		case SpaceBuster::ItemType::Ammo_reg:  //increase regular ammo reserves
			//user->ammoCountPrimary += (int) value;
			break;
		case SpaceBuster::ItemType::Ammo_Reflect: //increase reflection ammo reserves
			//user->ammoCountSecondary += (int) value;
			break;
		case SpaceBuster::ItemType::currency:
			printf("Item::cpp::\tUse currency by giving it away, duh!\n");
			break;
		case SpaceBuster::ItemType::Upgrade_speed:
		case SpaceBuster::ItemType::Upgrade_health:
		case SpaceBuster::ItemType::Upgrade_fireRate:
		case SpaceBuster::ItemType::Upgrade_defense:
		case SpaceBuster::ItemType::Upgrade_energyReduction:
		case SpaceBuster::ItemType::Upgrade_energyTorqueReduction:
		case SpaceBuster::ItemType::tool_winch:
		case SpaceBuster::ItemType::tool_forcefield:
		case SpaceBuster::ItemType::tool_gravityBomb:
		case SpaceBuster::ItemType::null_item:
		default:
			printf("Item::cpp::\tNot a valid Type!\n");
			break;
	}
	return true;
}



