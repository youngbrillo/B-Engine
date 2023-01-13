#include "Surface.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


const float cubeVerts[] = {
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

Surface::Surface()
    :VAO(0), VBO(0)
{
}

Surface::~Surface()
{
    //printf("Surface.cpp::~Surface::\tDeleting Surface: %i\n", VAO);  
    glDeleteVertexArrays(1, &VAO); 
    glDeleteBuffers(1, &VBO);
    glBindVertexArray(0);
}

void Surface::unBind()
{
    glBindVertexArray(0);
}

void quadSurface::Generate(float texCoordMin, float texCoordMax)
{
    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   texCoordMax, texCoordMax, // top right
         0.5f, -0.5f, 0.0f,   texCoordMax, texCoordMin, // bottom right
        -0.5f, -0.5f, 0.0f,   texCoordMin, texCoordMin, // bottom left
        -0.5f,  0.5f, 0.0f,   texCoordMin, texCoordMax  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


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


    //printf("Generating Quad surface: %i\n", VAO);
}

void quadSurface::Generate(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef)
{
    if (VAO > 0)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   toprig.x, toprig.y, // top right
         0.5f, -0.5f, 0.0f,   botrig.x, botrig.y, // bottom right
        -0.5f, -0.5f, 0.0f,   botlef.x, botlef.y, // bottom left
        -0.5f,  0.5f, 0.0f,   toplef.x, toplef.y  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


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

void quadSurface::Bind()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); //unbind vertex array

}

void cubeSurface::Generate(float min, float max)
{


    const unsigned int posOff = 3, texOff = 2, normalOff = 3;
    float cubeVertWNormals[] = {
        //position              //texture mapping   //normals           
        -0.5f, -0.5f, -0.5f,    min, min,           0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,    max, min,           0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    max, max,           0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    max, max,           0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,    min, max,           0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,    min, min,           0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,    min, min,           0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,    max, min,           0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    max, max,           0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    max, max,           0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,    min, max,           0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    min, min,           0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,    min, min,           -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    max, min,           -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    max, max,           -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    max, max,           -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    min, max,           -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    min, min,           -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,    min, min,           1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    max, min,           1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    max, max,           1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    max, max,           1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    min, max,           1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    min, min,           1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,    min, min,           0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    max, min,           0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    max, max,           0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    max, max,           0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    min, max,           0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    min, min,           0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,    min, min,           0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    max, min,           0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    max, max,           0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    max, max,           0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    min, max,           0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    min, min,           0.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertWNormals), cubeVertWNormals, GL_STATIC_DRAW);

    unsigned int strideMag = posOff + texOff + normalOff;

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideMag * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strideMag * sizeof(float), (void*)(posOff * sizeof(float)));
    glEnableVertexAttribArray(1);
    //normal coord attributes
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, strideMag * sizeof(float), (void*)((posOff + texOff) * sizeof(float)));
    glEnableVertexAttribArray(2);

    //printf("Generating Cube surface: %i\n", VAO);

}

void cubeSurface::Bind()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
