/*
	notes:
		I want there to be 2-3 basic UI functions:
		- UI Text that appears in NDC Space (For now..)
		- UI Meters that can show releative values of int's or floats
		- UI images (appear in NDC space too)
*/
#pragma once
#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include "Shader.h"
#include "Surface.h"
#include <glm/glm.hpp>
#include <string>
#include "Texture.h"
//ui element abstract class
struct UIElementDefinition
{
	//transform properties
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 pivot = glm::vec3(-0.5f, -0.5f, 0.0f); //set pivor/anchor to the bottom left corner
	glm::vec3 size = glm::vec3(1.0f);
	glm::vec3 meterSize = glm::vec3(15.0f, 1.0f, 1.0f);
	float scale = 1.0f;
	float rotation = 0.0f;

	//color properties
	glm::vec4 color = glm::vec4(1.0f);
	glm::vec4 backgroundColor = glm::vec4(0.550f, 0.550f, 0.550f, 1.0f);
	glm::vec4 outlineColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);


	//misc. properties
	std::string elementName = "Unnamed UI Element";
};

class UIElement
{
public:
	UIElement();
	UIElement(const UIElementDefinition* def);
	virtual ~UIElement();

	virtual void Draw(Shader* customShader = nullptr, Surface* customSurface = nullptr) const = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Debug();
protected:
	//screen position
	glm::vec3 position;
	//where the element's base lies
	glm::vec3 pivot;
	//dimensions of the element
	glm::vec3 size;
	//color of the element
	glm::vec4 color;
	//color of any appliciable background color
	glm::vec4 backgroundColor;
	//color of any outline 
	glm::vec4 outlineColor;
	//uniform scaling factor
	float scale;
	//orientation of UI element, locked to the z-axis
	float rotation;
	std::string name;
	//shader that all UI elements will use unless
	static Shader* sharedShader;
	static Surface* sharedSurface;

public: 
	static Texture* defaultTexture;

};

#endif // !UI_ELEMENTS_H