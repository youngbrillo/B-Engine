#include "t_Surface.h"

#include <iostream>

#include <glad/glad.h>
//#include <GLFW/glfw3.h>


const unsigned int DrawModes[7] =
{ GL_POINT, GL_LINE, GL_LINE_LOOP, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN };
const char* DrawModeNames[7] =
{ "GL_POINT", "GL_LINE", "GL_LINE_LOOP", "GL_LINE_STRIP", "GL_TRIANGLES", "GL_TRIANGLE_STRIP", "GL_TRIANGLE_FAN"};


t_Surface::t_Surface(bool generateOk)
	: vertex_array_object(0)
	, vertex_buffer_object(0)
	//, vertexArray(nullptr)
	, vertexArraySize(0)
	, enabled(false)
    , mode(GL_TRIANGLES)
    , modeIndex(4)
{
	if (generateOk)
		Generate(0, 1);
}

t_Surface::~t_Surface()
{
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);
	glBindVertexArray(0);

}

void t_Surface::Bind()
{
	glBindVertexArray(vertex_array_object);
	glDrawElements(mode, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); //unbind vertex array
}

void t_Surface::unBind()
{
	glBindVertexArray(0);
}
void t_Surface::Generate(float texCoordMin, float texCoordMax)
{
    if (enabled) { printf("I will not generate a surface twice >:(....It's expensive...\n"); return; }
    enabled = true;
    //vertexArray = new float[20];
    float vertexArray[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   texCoordMax, texCoordMax, // top right
         0.5f, -0.5f, 0.0f,   texCoordMax, texCoordMin, // bottom right
        -0.5f, -0.5f, 0.0f,   texCoordMin, texCoordMin, // bottom left
        -0.5f,  0.5f, 0.0f,   texCoordMin, texCoordMax  // top left 
    };


    //printf("verticies size: %d, %d, %d\n", sizeof(vertexArray), sizeof(vertexArray[0]), sizeof(float));
    //printf("verticies size: %d, %d, %d\n", sizeof(vertices), sizeof(vertices[0]), sizeof(float));

    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int EBO;
    glGenVertexArrays(1, &vertex_array_object);
    glGenBuffers(1, &vertex_buffer_object);
    glGenBuffers(1, &EBO);

    glBindVertexArray(vertex_array_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void t_Surface::Generate(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef)
{
    printf("not implemented yet\n"); return;
}

void t_Surface::Generate(int index)
{
    printf("not implemented yet\n"); return;
}

#include <imgui.h>

void t_Surface::Debug(const char* name)
{
    if (ImGui::TreeNode(name))
    {

        if (ImGui::SliderInt("Draw Mode", &modeIndex, 0, 6, DrawModeNames[modeIndex]))
        {
            mode = DrawModes[modeIndex];
        }
        ImGui::TreePop();
    }
}
