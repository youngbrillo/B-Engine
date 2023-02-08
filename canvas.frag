#version 330 core

out vec4 Fragcolor;
in vec2 TexCoords;

uniform sampler2D image;
uniform vec4 Color;

void main()
{    
	Fragcolor = Color * texture(image, TexCoords);
}  