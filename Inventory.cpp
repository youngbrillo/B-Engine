#include "Inventory.h"
#include "ResourceManager.h"

using namespace SpaceBuster;

Inventory::Inventory()
	: screenPosition(-.423, 0)
	, screenExtents(0,0)
	, itemSize(0.068f, .115f )
	, itemoffset(0.01)
	, visible(true)
	, backgoundTexture(nullptr)
	, backGroundPos(0, -0.5f, 0)
	, backgroundSize(1.0f, 0.5f, 1.0f)
	, backgroundColor(0.0f, 0.0f, 0.0f, 1.0f)
	, itemSlotColor(1.0f)
	, itemScale(1.0f)
{
	items.clear();

	backgoundTexture = new Texture("icon.png");
}

Inventory::~Inventory()
{
	printf("Deleting Items................\n");
	auto itr = items.begin();
	while (itr != items.end())
	{
		auto i = (*itr);
		itr = items.erase(std::remove(items.begin(), items.end(), i), items.end());
		delete i;
		//RemoveItem((*itr));
	}
	items.clear();

	delete backgoundTexture;
	printf("................Deleted Items\n");
}

void Inventory::useItem(itemObjectLite* i)
{
	float placeholderValue = 10.0f;
	i->m_item->Use(i->value);
	--i->amount;
	if (i->amount <= 0)
		RemoveItem(i);

}

void SpaceBuster::Inventory::useItem(SpaceBuster::Item* i)
{
	//get the itemObject
	for (auto e : items)
	{
		if (e->m_item->type == i->type)
		{
			useItem(e);
			break;
		}
	}

}

void Inventory::AddItem(Item* i)
{
	//lazy find
	bool earlyEnd = false;
	for (auto e : items)
	{
		if (e->m_item->type == i->type && i->canStack)
		{
			e->amount++;
			earlyEnd = true;
			printf("Adding additonal %s: %d\n", i->name.c_str(), e->amount);
		}
	}

	if (earlyEnd) return;
	Texture* itemTexture = ResourceManager::GetTexturePtr(ResourceManager::lookupItemToTexture[i->GetType()]);
	itemObjectLite* iol = new itemObjectLite( i, itemTexture,  1, 10.0f );

	items.emplace_back(iol);
}

//void SpaceBuster::Inventory::AddItem(ItemObject* i)
//{
//	printf("NOt implemented!\n");
//	return;
//	//lazy find
//	bool earlyEnd = false;
//	for (auto e : items)
//	{
//		if (e->m_item->type == i->itemReference->type && i->itemReference->canStack)
//		{
//			e->amount++;
//			earlyEnd = true;
//			printf("Adding additonal %s: %d\n", i->name.c_str(), e->amount);
//		}
//	}
//
//	if (earlyEnd) return;
//
//	Texture* itemTexture = ResourceManager::GetTexturePtr(ResourceManager::lookupItemToTexture[i->itemReference->type]);
//	itemObjectLite* iol = new itemObjectLite(i->itemReference, itemTexture, 1, 10.0f);
//
//	items.emplace_back(iol);
//}

void Inventory::RemoveItem(itemObjectLite* i)
{
	items.erase(std::remove(items.begin(), items.end(), i), items.end());
	delete i;
}
const glm::mat4 MODEL_MATRIX = glm::mat4(1);

//write a UI wrapper for latter....
//void SpaceBuster::Inventory::Draw(Shader* ndcShader)
//{
//	if (!visible) return;
//	glm::mat4 model(1);
//	model = glm::translate(model, backGroundPos);
//	model = glm::scale(model, backgroundSize);
//
//	ndcShader->Use()
//		.SetMatrix4("model", model)
//		.SetVector4f("color", backgroundColor);
//
//	backgoundTexture->Bind();
//	GameObject::g_GameObjectSurface->Bind();
//	for (int i = 0; i < items.size(); i++)
//	{
//		DrawInventoryItem(items[i], ndcShader, i);
//	}
//
//
//
//	//unbind
//	backgoundTexture->Unbind();
//	GameObject::g_GameObjectSurface->unBind();
//}
//
//void SpaceBuster::Inventory::DrawInventoryItem(itemObjectLite* i, Shader* ndcShader, int index)
//{
//	glm::mat4 model(1);
//	float xOff = backGroundPos.x + screenPosition.x + ((itemoffset.x + (itemSize.x * itemScale)) * index);
//	float yOff = backGroundPos.y + screenPosition.y;
//
//	model = glm::translate(model, glm::vec3(xOff, yOff, 0));
//	model = glm::scale(model, glm::vec3(itemSize * itemScale, 0));
//
//	ndcShader->Use()
//		.SetMatrix4("model", model)
//		.SetVector4f("color", itemSlotColor);
//
//	i->m_texture->Bind();
//	GameObject::g_GameObjectSurface->Bind();
//}


#include <imgui.h>

void Inventory::Debug()
{
	if (ImGui::TreeNode("-Inventory debug-"))
	{
		//if (ImGui::TreeNode("-Inventory-"))
		//{
			for (auto item : items)
			{
				if (ImGui::TreeNode(item->m_item->name.c_str()))
				{
					//item desc
					// 
					//item texture
					if (ImGui::Button("Use"))
					{
						useItem(item);
						//if(item->m_item->Use(item->value))
							//item->amount--;
					}
					//amount
					ImGui::SliderInt("amount", &item->amount, 0, 0);
					//item value
					ImGui::TreePop();
				}
			}

		//	ImGui::TreePop();
		//}
		/*	
		if (ImGui::TreeNode("-screen manip-"))
		{
			ImGui::SliderFloat2("Screen position", &screenPosition.x, -1, 1);
			ImGui::SliderFloat2("Screen extents", &screenExtents.x, -1, 1);
			ImGui::SliderFloat2("item size", &itemSize.x, -1, 1);
			ImGui::SliderFloat("item scale", &itemScale, .01f, 5);
			ImGui::SliderFloat2("item offset", &itemoffset.x, -1, 1);
			ImGui::ColorEdit4("item slot Color", &itemSlotColor.x, ImGuiColorEditFlags_Float);

			ImGui::TreePop();
		}

		backgoundTexture->RenderDebug();
		ImGui::SliderFloat3("bg position", &backGroundPos.x, -1, 1);
		ImGui::SliderFloat3("bg size", &backgroundSize.x, -1, 1);
		ImGui::ColorEdit4("bg Color", &backgroundColor.x, ImGuiColorEditFlags_Float);
		*/

		ImGui::TreePop();
	}
}

