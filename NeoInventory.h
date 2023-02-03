#pragma once
#include "Item.h"
#include "Transform.h"
#include <vector>


#include "Shader.h"
#include "Surface.h"

#include "UI_Button.h"
#include <glm/glm.hpp>

class NeoShipController; //forward reference

class NeoItem
{
public:
	NeoItem();
	NeoItem(UIElementDefinition* def, SpaceBuster::Item* i);
	~NeoItem();

	void onUse();

	void Update(float dt);
	void Draw(Shader* shader, Surface* surface);

	void handleEvent_mouseClick(const glm::vec2& screen_space_mouse_pos, int button, int action, int mode);
	void Debug();

public:

	ItemUIButton button;
	SpaceBuster::Item* m_item;
	int amount;
	bool removeFlag;

};


class NeoInventory
{
public:
	NeoInventory();
	~NeoInventory();


	void useItem(NeoItem* i);
	void useItem(SpaceBuster::Item* i);

	void AddItem(SpaceBuster::Item* i);
	std::vector<NeoItem*>::iterator RemoveItem(NeoItem* i);
	void Update(float dt);
	void Draw(Shader* shader, Surface* surface);


	void handleEvent_mouseClick(const glm::vec2& screen_space_mouse_pos, int button, int action, int mode);
	void Debug();
public:

	std::vector<NeoItem*> items;
	glm::vec3 screenPosition, itemoffset;

	UIElementDefinition buttonDefinition;

	NeoShipController* shipControllerRef;
};

