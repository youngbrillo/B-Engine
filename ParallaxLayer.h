#pragma once
#ifndef PARALLAXLAYER_H
#define PARALLAXLAYER_H

#include "Texture.h"
#include "Sprite.h"
#include <glm/glm.hpp>
#include <iostream>
class ParallaxLayer
{
public:
	ParallaxLayer(Texture* texture, int order, float speed, std::string name);
	~ParallaxLayer();
	
	Sprite* layer;
	//scroller corners;
	bool visible;
	int ordinal;
	float transitionSpeed;
	glm::vec2 a, b, c, d;
	glm::vec2 origin, offset;
		
	void updateVectors(bool autoConfigure = false);
	void Debug();

	std::string title;

};
#endif // !PARALLAXLAYER_H
