#pragma once
#include "UI_element.h"
#include "Texture.h"

#include "Item.h"
class UIButton : public UIElement
{
public: //functions
	UIButton();
	UIButton(UIElementDefinition* def, Texture* t = nullptr);
	virtual ~UIButton();

	virtual void Draw(Shader* customShader = nullptr, Surface* customSurface = nullptr) const override;
	virtual void Update(float deltaTime) override;
	virtual void Debug() override;

public: //non inherited functions
	virtual void onClick(int mouseKey, bool isReleased);
	bool isPointInBounds(glm::vec2 screen_space_point);

public: //members
	bool active, clicked, enabled;
	Texture* textureRef;
};

class ItemUIButton : public UIButton
{
public:
	ItemUIButton() : UIButton(), itemRef(nullptr) {};
	ItemUIButton(UIElementDefinition* def, SpaceBuster::Item* ref, Texture* t = nullptr) : UIButton(def, t), itemRef(ref) {};
	~ItemUIButton() { itemRef = nullptr; };


	virtual void onClick(int mouseKey, bool isReleased) override;
public:
	SpaceBuster::Item* itemRef;
};