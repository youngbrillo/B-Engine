#pragma once
#include <glm/glm.hpp>
#include<vector>
#include "Transform.h"
#include "Shader.h"
#include "Surface.h"

class CanvasItem;//forward reference

class Canvas
{
	public:
	enum RowAlignment
	{
		RowTop = 1,
		RowCenter = 0,
		RowBottom = -1
	};
	enum ColumnAlignment
	{
		ColLeft = -1,
		ColCenter =0,
		ColRight = 1,
	};
	public:
		Canvas();
		~Canvas();

		void Update(float dt);
		void Draw(Shader* shader, Surface*);

		void Debug(const char* canvasName = "Canvas");

	public:
		glm::vec2 Dimensions;
		Transform t;
		std::vector<CanvasItem*> children;
		RowAlignment rowAlignment;
		ColumnAlignment columnAlignment;
};

