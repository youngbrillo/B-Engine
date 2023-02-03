#include "UI_Button.h"


UIButton::UIButton()
	: UIElement(), active(false), clicked(false), enabled(true), textureRef(nullptr)
{
	textureRef = defaultTexture;
}
UIButton::UIButton(UIElementDefinition* def, Texture* t)
	: UIElement(def)
	, active(false), clicked(false), enabled(true)
	, textureRef(t)
{
	if(!textureRef) textureRef = defaultTexture;
}

UIButton::~UIButton()
{
	textureRef = nullptr;
}

void UIButton::onClick(int mouseKey, bool isReleased)
{
	if(isReleased)
		active = !active;
}

bool UIButton::isPointInBounds(glm::vec2 screen_space_point)
{
	glm::vec2 bottom_left = this->position + this->pivot; //assume bottom left is the default pivot
	glm::vec2 top_right = glm::vec2((bottom_left.x * size.x) + scale, (bottom_left.y  * size.y) + scale);
	bool value = screen_space_point.y >= bottom_left.y && screen_space_point.y <= top_right.y
		&&
		screen_space_point.x >= bottom_left.x && screen_space_point.x <= top_right.x;

	//printf("Point A\t(%.3f, %.3f)\n", bottom_left.x, bottom_left.y);
	//printf("Point O\t(%.3f, %.3f)\n", screen_space_point.x, screen_space_point.y);
	//printf("Point B\t(%.3f, %.3f)\n", top_right.x, top_right.y);
	return value;
}

void UIButton::Update(float deltaTime)
{
	
}

void UIButton::Draw(Shader* customShader, Surface* customSurface) const
{
	Shader* _shader = customShader ? customShader : UIElement::sharedShader;
	Surface* _surface = customSurface ? customSurface : UIElement::sharedSurface;

	if (!_shader) { printf("UI_Meter.cpp::Draw\tI have no shader and you are drawing me!?\n"); return; }
	if (!_surface) { printf("UI_Meter.cpp::Draw\tI have no ~surface~ and you are drawing me!?\n"); return; }
	glm::mat4 model(1.0f);
	//1st, the background
	{
		model = glm::translate(model, pivot); //adjust pivot point
		model = glm::translate(model, this->position); //set position

		model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 0.0f, 1.0f)); //set rotate

		model = glm::scale(model, this->size * this->scale); //set scale
		model = glm::translate(model, -pivot); //undo pivot point 
	}
	
	_shader->Use()
		.SetMatrix4("model", model)
		.SetVector4f("color", active ? this->outlineColor : this->backgroundColor);

	defaultTexture->Bind();
	customSurface->Bind();



	_shader->SetVector4f("color", active ? this->outlineColor : this->color);
	textureRef->Bind();


	customSurface->Bind();
}

void UIButton::Debug()
{
	UIElement::Debug();
}

void ItemUIButton::onClick(int mouseKey, bool isReleased)
{
	UIButton::onClick(mouseKey, isReleased);

	if(itemRef && isReleased)
	{
		itemRef->Use();
	}
}
