#pragma once
#ifndef INVENTORY_h
#define INVENTORY_h

//#include "ResourceManager.h"
//#include "ItemObject.h"
#include "Item.h"
#include "Texture.h"
#include <vector>
namespace SpaceBuster {
	struct itemObjectLite
	{
		itemObjectLite(SpaceBuster::Item* i, Texture* t, int a, float v)
			: m_item(i)
			, m_texture(t)
			, amount(a)
			, value(v)
		{};

		~itemObjectLite()
		{
			m_texture = nullptr;
			m_item = nullptr;
		};

		Texture* m_texture;
		SpaceBuster::Item* m_item;
		int amount;
		float value;
	};

	class Inventory
	{
	public:
		Inventory();
		~Inventory();


		void useItem(SpaceBuster::itemObjectLite* i);
		void useItem(SpaceBuster::Item* i);

		void AddItem(SpaceBuster::Item* i);
		//void AddItem(ItemObject* i);
		void RemoveItem(SpaceBuster::itemObjectLite* i);

		//void Draw(Shader* ndcShader);
		//void DrawInventoryItem(itemObjectLite* i, Shader* ndcShader, int offset);

		void Debug();
	private:
		std::vector<SpaceBuster::itemObjectLite*> items;
		//Spaceship* user; //for right now the user is the ship controller

		//the view will be in NDC
		glm::vec2 screenPosition, screenExtents, itemSize, itemoffset;
	public:
		bool visible;
	private:
		Texture* backgoundTexture;
		glm::vec3 backGroundPos, backgroundSize;
		glm::vec4 backgroundColor, itemSlotColor;
		float itemScale;
	};
}

#endif // !INVENTORY_h