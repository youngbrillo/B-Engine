#pragma once
#ifndef UI_METER_H
#define UI_METER_H

#include "UI_element.h"
#include <iostream>

enum UIElementInputType { UI_NOTYPE, UI_INT, UI_FLOAT, };


class UIMeter : public UIElement
{
public:
	UIMeter();
	UIMeter(UIElementDefinition* def);
	UIMeter(UIElementDefinition* def, float* input, float* min, float* max);
	UIMeter(UIElementDefinition* def, int* input,  int* min, int* max);
	~UIMeter();

	virtual void Draw(Shader* customShader = nullptr, Surface* customSurface = nullptr) const override;

	virtual void DrawValue(Shader* customShader = nullptr, Surface* customSurface = nullptr, float ratio = 1.0f);
	virtual void Update(float deltaTime) override;
	virtual void Debug() override;

private:
	UIElementInputType type;
public:
	float *value, *minValue, *maxValue;
	int* int_value, * int_minValue, * int_maxValue;
	
public:
	//for future consideration
	//void hitMaxValue(); //call this function on the frame when/after the value changes to the maxValue
	//void onMaxValue();	//call this function on all frames when the value is the maxValue
	//void hitMinValue();  //call this function on the frame when/after the value changes to the minValue
	//void onMinValue();  //call this function on all frames when the value is the minValue
};

#endif // !UI_METER_H