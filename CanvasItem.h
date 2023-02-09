#pragma once

#include "Transform.h"
#include "Shader.h"
#include "Surface.h"
#include "Texture.h"
#include "TextRenderer.h"
#include <string>
#include <functional>

struct Callback{
	template <typename Function, typename... Args>
	Callback(Function& func, Args&&... args) {
		auto f = std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
		func_ = [f] {f(); };
	}
	void evaluate() { func_(); }
	std::function<void()> func_;
};

class CanvasItem
{
public:
	CanvasItem() : transform(), texture(nullptr), Color(glm::vec4(1.0f)) {};
	CanvasItem(Texture* t, const glm::vec2& position = glm::vec2(0.0f), glm::vec4 color = glm::vec4(0.5f), bool v = true, bool s = true) : visible(v), selectable(s), transform(), texture(t), Color(color), activeColor(1.0f), active(false) { transform.position = glm::vec3(position, 0.0f); };
	virtual ~CanvasItem() {
		texture = nullptr; 
		if (func)
		{
			delete func;
		}
		func = nullptr;
	};

	virtual void Update(float dt, Transform* t) {};

	virtual void DrawTexture(Shader* shader, const Transform& _trans);
	virtual void Draw(Shader* shader, Surface* surface, const glm::vec2& offsetPosition = glm::vec2(0.0f));
	virtual void Draw(Shader* shader, Surface* surface, const Transform& parentTransform, const glm::vec2& offsetPosition = glm::vec2(0.0f));
	virtual void Debug(const char* nodeName = "Canvas Item");

	virtual void handleCallback() {}
	void setSelectCallback(void(*ptr)())
	{ 
		//auto f1 = std::bind(ptr);
		func = new Callback(ptr);
	}
	virtual bool isPointInBounds(const glm::vec2& pos, Transform* parent) { return false; }
public:
	Transform transform;
	glm::vec4 Color, activeColor; 
	Texture* texture;
	static TextRenderer* canvasText;
	static glm::vec2 mousePosition;
	bool active, visible, selectable;
public:
	Callback* func = nullptr;
};

class CanvasText : public CanvasItem
{
public:
	CanvasText() : CanvasItem(), Text("[Enter Text Here"), offset(0.0f) {};
	CanvasText(std::string string, const glm::vec2& position = glm::vec2(0.0f), glm::vec4 color = glm::vec4(1.0f), bool v = true, bool s = true);// : CanvasItem(nullptr, position, color), Text(string), offset(0.0f) { };
	virtual ~CanvasText();

	//virtual void Draw(Shader* shader, Surface* surface, const glm::vec2& offsetPosition = glm::vec2(0.0f)) override;
	virtual void Update(float dt, Transform* t) override;
	virtual void Draw(Shader* shader, Surface* surface, const Transform& parentTransform, const glm::vec2& offsetPosition = glm::vec2(0.0f)) override;
	virtual void Debug(const char* nodeName = "Canvas Text") override;

	void handleCallback() override;
	void ResizeBounds();
	virtual bool isPointInBounds(const glm::vec2& pos, Transform* parent) override;

public:
	std::string Text;
	glm::vec2 offset;
	glm::vec2 bounds;//, debugBounds01, debugBounds02;
	glm::vec4 fontColor;
	bool viewBounds;
	bool saveBounds;
};
