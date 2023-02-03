#include "NeoInventory.h"
#include "ResourceManager.h"
#include "NeoShipController.h"

NeoItem::NeoItem() 
	: m_item(nullptr)
	, button()
	, amount(0)
	, removeFlag(false)
{
}

NeoItem::NeoItem(UIElementDefinition* def, SpaceBuster::Item* i)
	: m_item(i)
	, button(def, i, ResourceManager::GetTexturePtr(ResourceManager::lookupItemToTexture[i->GetType()]))
	, amount(1)
	, removeFlag(false)
{
}

NeoItem::~NeoItem()
{
	m_item = nullptr;
}

void NeoItem::onUse()
{
	amount--;

	if (amount <= 0) 
	{
		removeFlag = true;
	}
}


//
// 
// 
// 
//

NeoInventory::NeoInventory()
	: shipControllerRef(nullptr)
{

	screenPosition = glm::vec3(-42.0f, -25.0f, 0.0f);
	itemoffset = glm::vec3(5.0f, 0.0f, 0.0f);
	buttonDefinition.elementName = "some button";
	buttonDefinition.position = screenPosition;
	buttonDefinition.scale = 4.0f;
}

NeoInventory::~NeoInventory()
{
	auto itr = items.begin();
	while (itr != items.end())
	{
		auto obj = (*itr);

		itr = this->RemoveItem(obj);

		obj = nullptr;
	}
}

void NeoInventory::useItem(NeoItem* i)
{
	switch (i->m_item->GetType())
	{
	case SpaceBuster::ItemType::SparePart:
		//health restorative
		this->shipControllerRef->GetShip()->GetAttributes()->UpdateCondition(10.0f);
		break;
	case SpaceBuster::ItemType::EnergyCell:
		//energy restorative
		this->shipControllerRef->GetShip()->GetAttributes()->UpdateEnergy(20.0f);
		break;
	case SpaceBuster::ItemType::Upgrade_fireRate:
		//inc launcher's level
		this->shipControllerRef->m_launcher->level += 1;
		break;
	case SpaceBuster::ItemType::Upgrade_speed:
		//switch launcher fire mode to spread
		this->shipControllerRef->m_launcher->ToggleFireMode();// -= 0.01;
		break;
	default:
		printf("YOu are wanting to use an Inventory Item, but it has resulted in behavior that is undefined\n");
		break;
	}
	i->onUse();

}

void NeoInventory::useItem(SpaceBuster::Item* i)
{
	switch (i->GetType())
	{
	case SpaceBuster::ItemType::null_item:
	case SpaceBuster::ItemType::SparePart:
	case SpaceBuster::ItemType::EnergyCell:
	case SpaceBuster::ItemType::Ammo_reg:
	case SpaceBuster::ItemType::Ammo_Reflect:
	case SpaceBuster::ItemType::Upgrade_speed:
	case SpaceBuster::ItemType::Upgrade_health:
	case SpaceBuster::ItemType::Upgrade_fireRate:
	case SpaceBuster::ItemType::Upgrade_defense:
	case SpaceBuster::ItemType::Upgrade_energyReduction:
	case SpaceBuster::ItemType::Upgrade_energyTorqueReduction:
	case SpaceBuster::ItemType::tool_winch:
	case SpaceBuster::ItemType::tool_forcefield:
	case SpaceBuster::ItemType::tool_gravityBomb:
	case SpaceBuster::ItemType::currency:
	default:
		printf("YOu are wanting to use an Inventory Item, but it has resulted in behavior that is undefined\n");
		break;
	}
}

void NeoInventory::AddItem(SpaceBuster::Item* i)
{
	//lazy find
	bool earlyEnd = false;
	for (auto e : items)
	{
		if (e->m_item->GetType() == i->GetType() && i->canStack)
		{
			e->amount++;
			earlyEnd = true;
			printf("Adding additonal %s: %d\n", i->GetName().c_str(), e->amount);
		}
	}

	if (earlyEnd) return;

	buttonDefinition.elementName = "Button : " + i->GetName();
	NeoItem* iol = new NeoItem(&buttonDefinition, i);
	items.emplace_back(iol);

	buttonDefinition.position += itemoffset;
}

std::vector<NeoItem*>::iterator NeoInventory::RemoveItem(NeoItem* i)
{
	std::vector<NeoItem*>::iterator nextpos = items.erase(std::remove(items.begin(), items.end(), i), items.end());
	delete i;
	buttonDefinition.position -= itemoffset;

	return nextpos;

}
//
//  updates
//


void NeoInventory::Update(float dt) { for (auto i : items) { 	i->Update(dt); } }
void NeoItem::Update(float dt) {}

void NeoInventory::Draw(Shader* shader, Surface* surface) { for (auto i : items) { i->Draw(shader, surface); } }


void NeoItem::Draw(Shader* shader, Surface* surface)
{
	button.Draw(shader, surface);
}
#include <GLFW/glfw3.h>
void NeoInventory::handleEvent_mouseClick(const glm::vec2& UI_space_point, int button, int action, int mode)
{
	if (action != GLFW_RELEASE) return;
	auto itr = items.begin();
	//for (auto i : items)
	//{
	//	//i->handleEvent_mouseClick(UI_space_point, button, action, mode);
	//	if (i->button.isPointInBounds(UI_space_point))
	//	{
	//		this->useItem(i);
	//	}
	//}
	while (itr != items.end())
	{
		auto i = (*itr);
		if (i->button.isPointInBounds(UI_space_point))
		{
			this->useItem(i);
		}
		if (i->removeFlag)
		{
			itr = RemoveItem(i);
		}
		else
		{
			itr++;
		}
	}

}

void NeoItem::handleEvent_mouseClick(const glm::vec2& UI_space_point, int button, int action, int mode)
{

	if (this->button.isPointInBounds(UI_space_point))
	{
		this->button.onClick(button, action == GLFW_RELEASE);
		printf("todo: you may want to change this...\n");
	}
}

/// <summary>
/// debug visualizer 
/// </summary>
#include <imgui.h>
void NeoInventory::Debug()
{
	if (ImGui::TreeNode("Neo Inventory"))
	{
		for (auto i : items) 
		{ 
			i->Debug(); 
		}

		ImGui::TreePop();
	}
}


void NeoItem::Debug()
{
	std::string name = "item : " + m_item->GetName();


	if (ImGui::TreeNode(name.c_str()))
	{

		if (ImGui::Button("Use"))
		{
			printf("I do nothing so far...\n");
			//if(item->m_item->Use(item->value))
				//item->amount--;
		}
		this->button.Debug();
		//amount
		ImGui::SliderInt("amount", &amount, 0, 0);
		ImGui::TreePop();
	}
}
