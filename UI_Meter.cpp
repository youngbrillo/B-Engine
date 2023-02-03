#include "UI_Meter.h"

UIMeter::UIMeter() : UIElement(), type(UIElementInputType::UI_FLOAT), value(nullptr), minValue(nullptr), maxValue(nullptr)
{
}

UIMeter::UIMeter(UIElementDefinition* def)
	: UIElement(def)
	, type(UIElementInputType::UI_NOTYPE)
	, value(NULL), minValue(NULL), maxValue(NULL)
	, int_value(NULL), int_minValue(NULL), int_maxValue(NULL)

{
	size = def->meterSize;
}

UIMeter::UIMeter(UIElementDefinition* def, float* input, float* min, float *max)
	: UIElement(def)
	, type(UIElementInputType::UI_FLOAT)
	, value(input), minValue(min), maxValue(max)
	, int_value(NULL), int_minValue(NULL), int_maxValue(NULL)
{

	size = def->meterSize;
}

UIMeter::UIMeter(UIElementDefinition* def, int* input, int* min, int* max)
	: UIElement(def)
	, type(UIElementInputType::UI_INT)
	, value(NULL), minValue(NULL), maxValue(NULL)
	, int_value(input), int_minValue(min), int_maxValue(max)
{
	size = def->meterSize;
}

UIMeter::~UIMeter()
{
	int_value = NULL; int_minValue = NULL; int_maxValue = NULL;
	value = NULL; minValue = NULL; maxValue = NULL;
}


void UIMeter::Draw(Shader* customShader, Surface* customSurface) const
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
		.SetVector4f("color", this->backgroundColor);

	defaultTexture->Bind();

	customSurface->Bind();
	
	float ratio = 1.0f;

	model = glm::mat4(1.0f);
	//then, the actual value
	{
		model = glm::translate(model, pivot);//stays the same
		model = glm::translate(model, this->position); //stays the same
		model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 0.0f, 1.0f)); //stays the same

		//change the scaling based on the ratio!
		ratio = type == UIElementInputType::UI_FLOAT ? ( * value / *maxValue) : (*int_value / (* int_maxValue * 1.0f));
		model = glm::scale(model, glm::vec3(size.x * ratio, size.y, size.z) * this->scale);


		model = glm::translate(model, -pivot); //stays the same
	}

	_shader->Use()
		.SetMatrix4("model", model)
		.SetVector4f("color", this->color);

	defaultTexture->Bind();

	customSurface->Bind();
}

void UIMeter::DrawValue(Shader* customShader, Surface* customSurface, float ratio)
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
		.SetVector4f("color", this->backgroundColor);


	customSurface->Bind();

	model = glm::mat4(1.0f);
	//then, the actual value
	{
		model = glm::translate(model, pivot);//stays the same
		model = glm::translate(model, this->position); //stays the same
		model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 0.0f, 1.0f)); //stays the same

		//change the scaling based on the input ratio!
		model = glm::scale(model, glm::vec3(size.x * ratio, size.y, size.z) * this->scale);


		model = glm::translate(model, -pivot); //stays the same
	}

	_shader->Use()
		.SetMatrix4("model", model)
		.SetVector4f("color", this->color);


	customSurface->Bind();
}

void UIMeter::Update(float deltaTime)
{

}

void UIMeter::Debug()
{
	UIElement::Debug();
}
