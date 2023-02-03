#pragma once
#include "UI_element.h"
#include "UI_Meter.h"
#include "UI_Button.h"

#include "NeoShipController.h"
#include "Shader.h"
//presents the UI for the ship, by way of it's controller

class ShipControllerUI
{
public:
	ShipControllerUI(NeoShipController* shipManager);
	~ShipControllerUI();


	//draw controller UI Last!!
	void Draw(Shader* shader, Surface* surface);
	void DrawStatus(Shader* shader, Surface* surface);
	void DrawInventory(Shader* shader, Surface* surface);
	glm::vec2 getUISpacePoint(const glm::vec2 screenPoint);
	void HandleMouseClick(const glm::vec2& screen_space_mouse_pos, int button, int action, int mode);
	void HandleButtonPress(int key, bool keyReleased);
	void Debug();


public://references
	bool statsVisible, InventoryVisible;
	NeoShipController* controller;
	NeoInventory* inventory;

public://members
	UIMeter* healthMeter, * energyMeter;
	float minf, maxf;
	int minI, maxI;
	Shader* uiShader;// = new Shader("UIShader00.vts", "test.frs", true);


	ItemUIButton t_button;

	glm::mat4 m_projection_matrix;
	glm::vec2 inventory_offset;
};

