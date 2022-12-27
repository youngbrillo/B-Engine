#pragma once

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "Texture.h"
#include "shader.h"
#include "Surface.h"
#include "Item.h"


class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture>   Textures;
    static std::map<SpaceBuster::ItemType, SpaceBuster::Item> Items;
    static std::map<SpaceBuster::ItemType, std::string> lookupItemToTexture;
    //static TextRenderer texRen;

    static void startup();
    static void cleanup();
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    // retrieves a stored sader
    static Shader GetShader(std::string name);
    static Shader* GetShaderPtr(std::string name);
    // loads (and generates) a texture from file
    static Texture LoadTexture(const char* file, bool alpha, std::string name);
    // retrieves a stored texture
    static Texture GetTexture(std::string name);
    static Texture* GetTexturePtr(std::string name);

    static SpaceBuster::Item LoadItem(SpaceBuster::ItemType key, std::string itemName, std::string description, bool autoUse, bool canStack = true);
    static SpaceBuster::Item GetItem(SpaceBuster::ItemType key);
    static SpaceBuster::Item* GetItemPtr(SpaceBuster::ItemType key);

    // properly de-allocates all loaded resources
    static void Clear();
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // loads and generates a shader from file
    // loads a single texture from file
        // the repective classes do that now
    static bool ranStartup;
};


#endif