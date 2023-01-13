#include "experimentalSurface.h"


#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
experimentalSurface::experimentalSurface()
    : Surface()
    , m_verticies(nullptr)
{

    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1,1, // top right
         0.5f, -0.5f, 0.0f,   1,0, // bottom right
        -0.5f, -0.5f, 0.0f,   0,0, // bottom left
        -0.5f,  0.5f, 0.0f,   0,1  // top left 
    };
    m_verticies = new float[20];
    for (int i = 0; i < 20; i++) {  m_verticies[i] = vertices[i]; }

 //   printf("experimental Surface:: member\t size(%d), first 4th element (%.2f)\nt", sizeof(m_verticies) , m_verticies[3]);
 //   printf("experimental Surface:: vert  \t size(%d), first 4th element (%.2f)\nt", sizeof(vertices), vertices[3]);
}

experimentalSurface::~experimentalSurface()
{
    delete m_verticies;
}

void experimentalSurface::Generate(float rows, float cols)
{
    //printf("experimentalSurface.cpp::\tDO NOT IMPLEMENT\n");
    return;

    //unsigned int EBO;
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);

    //glBindVertexArray(VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    ////position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    ////texture coord
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);


}

void experimentalSurface::Generate(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef)
{

    //if (VAO > 0)
    //{
    //    glDeleteVertexArrays(1, &VAO);
    //    glDeleteBuffers(1, &VBO);
    //    delete m_verticies;
    //}

    //float vertices[] = {
    //    // positions          // texture coords
    //     0.5f,  0.5f, 0.0f,   toprig.x, toprig.y, // top right
    //     0.5f, -0.5f, 0.0f,   botrig.x, botrig.y, // bottom right
    //    -0.5f, -0.5f, 0.0f,   botlef.x, botlef.y, // bottom left
    //    -0.5f,  0.5f, 0.0f,   toplef.x, toplef.y  // top left 
    //};
    //m_verticies = new float[20];
    //for (int i = 0; i < 20; i++) { m_verticies[i] = vertices[i]; }
    int vertices = 80;
     

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
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), m_verticies, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, 80, m_verticies, GL_STATIC_DRAW);


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


struct VertexData
{
    glm::vec2 position;
    glm::vec2 texCoord;
};


void experimentalSurface::Generate(int rows, int columns, int textureWidth, int textureHeight)
{
    //unsigned int mVertexDataBuffer, mIndexBuffers;
    int totalSprites = rows * columns;
    VertexData* vertexData = new VertexData[totalSprites * 4];
    //Allocate vertex data buffer name
    glGenBuffers(1, &mVertexDataBuffer);

    //Allocate index buffers names
    glGenBuffers(totalSprites, mIndexBuffers);

    //Go through clips
    GLfloat tW = textureWidth;
    GLfloat tH = textureHeight;

    GLuint spriteIndices[4] = { 0, 0, 0, 0 };

    //generate vbo and ibo for each frame
}

void experimentalSurface::Bind()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); //unbind vertex array
}

void experimentalSurface::UpdateVerticies(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef)
{
    m_verticies[3] = toprig.x;
    m_verticies[4] = toprig.y;

    m_verticies[8] = botrig.x;
    m_verticies[9] = botrig.y;

    m_verticies[13] = botlef.x;
    m_verticies[14] = botlef.y;

    m_verticies[18] = toplef.x;
    m_verticies[19] = toplef.y;

    //this->Generate(toprig, botrig, botlef, toplef);
}




































#include "stb_image.h"
//texture
experimentalTexture::experimentalTexture(const char* path, bool alpha, int rowCount, int columnCount)
    : id(0)
    , Wrap_S(GL_REPEAT)
    , Wrap_T(GL_REPEAT)
    , Filter_Min(GL_LINEAR)
    , Filter_Max(GL_LINEAR)
    , width(-1), height(-1), channelCount(-1)
    , rows(rowCount)
    , columns(columnCount)
{
    this->GenerateTexture(path, alpha, id, width, height, channelCount);
}

experimentalTexture::~experimentalTexture()
{
}

void experimentalTexture::Bind()
{
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);
}

void experimentalTexture::Bind(int index)
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void experimentalTexture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void experimentalTexture::GenerateTexture(const char* path, bool alphaChannel, unsigned int& textureId, int& _width, int& _height, int& numOfChannels, int expectedChannels)
{
    glGenTextures(1, &textureId);
    /*
    glBindTexture(GL_TEXTURE_2D, textureId); //og

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &_width, &_height, &numOfChannels, expectedChannels);

    GLint internalFormat = alphaChannel ? GL_RGBA : GL_RGBA;

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        unsigned int textures;
    }
    else {
        printf("Failed to load texture: '%s'\n", path);
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    */

    glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
    //allocate storage
    
    //upload pixel data
    unsigned char* data = stbi_load(path, &_width, &_height, &numOfChannels, expectedChannels);
    if (!data) { printf("Could not read file %s", path); return; }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, _width, _height, )

}

void experimentalTexture::RenderDebug()
{
}
