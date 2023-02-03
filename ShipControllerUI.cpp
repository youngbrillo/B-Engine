#include "ShipControllerUI.h"
#include "Game.h"
#include "ResourceManager.h"

void printMatrix(const glm::mat4& matrix)
{
	printf("{\n");
	for (int i = 0; i < 4; i++)
	{
		printf("\t");
		for (int j = 0; j < 4; j++)
		{
			printf("%.3f", matrix[i][j]);
			if (j != 3) printf(", ");
		}
		printf("\n");
	}
	printf("}\n");
}


ShipControllerUI::ShipControllerUI(NeoShipController* shipManager)
	: controller(shipManager)
	, inventory(&shipManager->m_inventory)
	, statsVisible(true)
	, InventoryVisible(true)
	, minf(0.0f), minI(0)
	, maxf(1.0f), maxI(100)
	, inventory_offset(5.0f, 0.0f)
{
	UIElementDefinition UIdef, buttonDef;
	Ship* ship = controller->m_ship;
	ShipAttributes* attrb = &ship->attributes;
	//health meter config
	{
		UIdef.position = glm::vec3(-42.0f, -20.0f, 0.0f);
		UIdef.elementName = "Health meter";
		UIdef.color = glm::vec4(0, 1.0f, 0.0f, 1.0f);
		healthMeter = new UIMeter(&UIdef, &attrb->currentCondition, &minf, &attrb->maxCondition);
		
	}
	//energy meter config
	{
		UIdef.position = glm::vec3(-24.0f, -20.0f, 0.0f);
		UIdef.elementName = "Energy meter";
		UIdef.color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		energyMeter = new UIMeter(&UIdef, &attrb->currentEnergy, &minf, &attrb->maxEnergy);
	}

	uiShader = new Shader("UIShader00.vts", "UIShader00.frs", true);
	glm::mat4 view = glm::mat4(1.0f);


	m_projection_matrix[0] = { 0.021f, 0.000f, 0.000f, 0.000f };
	m_projection_matrix[1] = { 0.000f, 0.038f, 0.000f, 0.000f };
	m_projection_matrix[2] = { 0.000f, 0.000f, -1.00f, 0.000f };
	m_projection_matrix[3] = { 0.000f, 0.000f, 0.000f, 1.000f };

	printMatrix(m_projection_matrix);

	//projection = Game::AppCam->getProjectionMatrix(); printMatrix(projection);

	uiShader->Use().SetMatrix4("view", view)
		.setInt("image", 0)
		.SetMatrix4("projection", m_projection_matrix);


	//test button definition
	{
		buttonDef.elementName = "Test button";
		buttonDef.scale = 2.0f;
		buttonDef.position = glm::vec3(-42.0f, -25.0f, 0.0f);
		buttonDef.scale = 4;
		t_button = ItemUIButton(&buttonDef, ResourceManager::GetItemPtr(SpaceBuster::ItemType::SparePart));
		t_button.textureRef = ResourceManager::GetTexturePtr("healthred");

	}
}

ShipControllerUI::~ShipControllerUI()
{
	delete healthMeter;
	delete energyMeter;
	delete uiShader;
	controller = nullptr;
	inventory = nullptr;
}
void ShipControllerUI::Draw(Shader* shader, Surface* surface)
{

	DrawStatus(uiShader, surface);
	DrawInventory(uiShader, surface);
}

void ShipControllerUI::DrawStatus(Shader* shader, Surface* surface)
{
	healthMeter->Draw(shader, surface);
	energyMeter->Draw(shader, surface);
}

void ShipControllerUI::DrawInventory(Shader* shader, Surface* surface)
{
	//UIElementDefinition buttonDef;

	////buttonDef.elementName = "inventory button";
	////buttonDef.position = glm::vec3(-42.0f, -25.0f, 0.0f);
	////buttonDef.scale = 4.0f;
	inventory->Draw(shader, surface);


	///t_button.Draw(shader, surface);
}

glm::vec2 ShipControllerUI::getUISpacePoint(const glm::vec2 screenPoint)
{
	//required variables
	glm::mat4 projection = m_projection_matrix;
	glm::mat4 view = glm::mat4(1.0f);
	glm::vec4 output;

	glm::vec4 NDC;

	{ //get normalized device coords
		float w, h, x, y, ndc_x, ndc_y;
		w = Game::AppCam->Width * 1.0f;
		h = Game::AppCam->Height * 1.0f;
		x = screenPoint.x;
		y = screenPoint.y;



		ndc_x = 2.0 * x / w - 1.0;
		ndc_y = 1.0 - 2.0 * y / h; //invert y axis

		NDC = glm::vec4(ndc_x, ndc_y, 0, 1);
	}

	{
		output = NDC * glm::inverse(projection) * glm::inverse(view);

	}

	return glm::vec2(output.x, output.y);
}

void ShipControllerUI::HandleMouseClick(const glm::vec2& screen_space_mouse_pos, int button, int action, int mode)
{
	glm::vec2 UI_space_point = getUISpacePoint(screen_space_mouse_pos);

	inventory->handleEvent_mouseClick(UI_space_point, button, action, mode);

	if (t_button.isPointInBounds(UI_space_point))
	{
		t_button.onClick(button, action == GLFW_RELEASE);
	}

}

void ShipControllerUI::HandleButtonPress(int key, bool keyReleased)
{

}
#include <imgui.h>
void ShipControllerUI::Debug()
{
	//ImGui::Checkbox("t1", &t1);
	if (ImGui::Button("Hot Reload UI shader"))
	{
		if(uiShader)
			delete uiShader;
		uiShader = new Shader("UIShader00.vts", "UIShader00.frs", true);

		uiShader->Use()
			.SetMatrix4("view", glm::mat4(1.0f))
			.SetMatrix4("projection", m_projection_matrix)
			.setInt("image", 0);
	}

	if(ImGui::TreeNode("Ship UI"))
	{
		ImGui::SliderFloat2("Invenotry offset", &inventory_offset.x, 0, 20.0f);

		t_button.Debug();

		healthMeter->Debug();
		energyMeter->Debug();

		ImGui::TreePop();
	}
}
