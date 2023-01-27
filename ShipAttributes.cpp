#include "ShipAttributes.h"

ShipAttributes::ShipAttributes(ShipAttributeDef* def)
	: ThrustForce(def->ThrustForce)
	, ThrustPosition(def->ThrustPosition)
	, TorqueMag(def->TorqueMag)
	, startingCondition(def->startingCondition)
	, maxCondition(def->maxCondition)
	, startingEnergy(def->startingEnergy)
	, maxEnergy(def->maxEnergy)
	, physicalDefense(def->physicalDefense)
	, energyCost(def->energyCost)
	, torqueRatio(def->torqueRatio)
	, impulseCostModifier(def->impulseCostModifier)
	, operational(true)
	, broken(false)
{
}

ShipAttributes::ShipAttributes(const char* attributeFile)
{
	LoadAttributeFile(attributeFile);
}

ShipAttributes::~ShipAttributes()
{
}

void ShipAttributes::LoadAttributeFile(const char* fileName)
{
	//open file


	//parse Json


	//Assign values to Definition
	ShipAttributeDef def;

	{

	}

	//assign member variables
	ThrustForce = def.ThrustForce;
	ThrustPosition = def.ThrustPosition;
	TorqueMag = def.TorqueMag;
	startingCondition = def.startingCondition;
	maxCondition = def.maxCondition;
	startingEnergy = def.startingEnergy;
	maxEnergy = def.maxEnergy;
	physicalDefense = def.physicalDefense;
	energyCost = def.energyCost;
	torqueRatio = def.torqueRatio;
	impulseCostModifier = def.impulseCostModifier;
	operational = true;
	broken = false;
}
#include <imgui.h>

void ShipAttributes::Debug(const char* name)
{
	if (ImGui::TreeNode(name))
	{

		//move to the bottom later....
		//ImGui::Checkbox("double tap listening?", &listeningForDoubleTap);
		//ImGui::Checkbox("double tap enabled?", &doubleTapEnabled);
		//ImGui::SliderFloat("DT time left", &doubleTapIntervalTimeLeft, 0, 0);
		//ImGui::SliderFloat("DT time limit", &doubleTapIntervalLimit, 0, 1.0f);
		//ImGui::SliderFloat("DT velocity", &doubleTappVelocityMod, 1.0f, 5.0f);
		//ImGui::SliderFloat("DT torque", &doubleTappTorqueMod, 1.0f, 5.0f);


		//.....thanks! ^_^
		ImGui::Checkbox("broken", &broken);
		ImGui::Checkbox("can move", &operational);

		ImGui::SliderFloat2("Forward Force", &ThrustForce.x, -300, 0);
		ImGui::SliderFloat2("Thrust pos ", &ThrustPosition.x, -5, 5);
		ImGui::SliderFloat("Torque Scalar", &TorqueMag, 0, 10);
		ImGui::SliderFloat("condition", &startingCondition, 0, 100.0f);
		ImGui::SliderFloat("defense (phys)", &physicalDefense, 0, 100.0f);
		ImGui::SliderFloat("energy", &startingEnergy, 0, 100.0f);
		ImGui::SliderFloat("energy Cost", &energyCost, 0, 100.0f);
		ImGui::SliderFloat("torque ratio", &torqueRatio, 0, 100.0f);






		ImGui::TreePop();
	}
}
