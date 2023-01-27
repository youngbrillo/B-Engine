#include "ResourceManager.h"
#include <glad/glad.h>

using namespace SpaceBuster;

// Instantiate static variables
std::map<std::string, Texture>  ResourceManager::Textures;
std::map<std::string, Shader>   ResourceManager::Shaders;
std::map<ItemType, Item>        ResourceManager::Items;
std::map<ItemType, std::string>  ResourceManager::lookupItemToTexture;
bool  ResourceManager::ranStartup = false;


#include <filesystem>

//namespace fs = std::filesystem;

void ResourceManager::startup()
{
	if (ranStartup) 
	{ 
		printf("ResourceManager.cpp::\tStartup has already run, run cleanup first to restart!\n");
		return; 
	}
	//todo: in the future have the system read through everything and yada yada yada but for now i'll do it by hand
	printf("ResourceManager.cpp::\tCreating assets\n");

	//load items
	ResourceManager::LoadItem(ItemType::SparePart, "Spare Part", "Restores Condition", true, true);
	ResourceManager::LoadItem(ItemType::EnergyCell, "Energy Cells", "Recover Energy", true, true);
	ResourceManager::LoadItem(ItemType::Ammo_reg, "3.108 mm", "Regular Ammunition", true, true);
	ResourceManager::LoadItem(ItemType::Ammo_Reflect, "Bouncing Bertha", "Reflective 1x Ammuntion", true, true);
	ResourceManager::LoadItem(ItemType::Upgrade_fireRate, "FireRate++", "Increase Fire rate", true, true);
	ResourceManager::LoadItem(ItemType::Upgrade_speed, "ShotGunSpread", "Shotgun spread", true, true);
	//load item textures
	//ResourceManager::LoadTexture("./assets/SparePart.png", true, "SparePart");
	//ResourceManager::LoadTexture("./assets/EnergyCell.png", true, "EnergyCell");
	//ResourceManager::LoadTexture("./assets/Ammo.png", true, "Ammo");
	ResourceManager::LoadTexture("./assets/brick.png", true, "icon");
	ResourceManager::LoadTexture("./assets/effects/health02.png", true, "healthred");
	ResourceManager::LoadTexture("./assets/effects/health01.png", true, "healthgreen");
	ResourceManager::LoadTexture("./assets/effects/accel.png", true, "accelIcon");
	ResourceManager::LoadTexture("./assets/effects/spread.png", true, "spreadIcon");
	ResourceManager::LoadTexture("./assets/bgs/blankpixel.png", true, "default");

	ResourceManager::lookupItemToTexture[ItemType::SparePart] = "healthred";
	ResourceManager::lookupItemToTexture[ItemType::EnergyCell] = "healthgreen";
	ResourceManager::lookupItemToTexture[ItemType::Ammo_reg] = "Ammo";
	ResourceManager::lookupItemToTexture[ItemType::Ammo_Reflect] = "Ammo";
	ResourceManager::lookupItemToTexture[ItemType::Upgrade_fireRate] = "accelIcon";
	ResourceManager::lookupItemToTexture[ItemType::Upgrade_speed] = "spreadIcon";

	


	//todo: //load shaders, but i'm not dealing w/ universal shaders right now so :/
	//when i have 2+ shaders that may/maynot be running every moment, THEN, i will add them here.
	//ResourceManager::LoadShader("test.vts", "test.frs", nullptr, "ColorShader");
	//ResourceManager::LoadShader("stdsprite.vts", "stdsprite.frs", nullptr, "SpriteShader");
	ranStartup = true;
}

void ResourceManager::cleanup()
{
	if (!ranStartup)
	{
		printf("ResourceManager.cpp::\tCleanup should not run, b/c startup has not run!\n");
		return;
	}
	printf("ResourceManager.cpp::\tClearing All assets\n");
	//lookupItemToTexture.clear();
	//Textures.clear();
	//Shaders.clear();
	//Items.clear();
	ResourceManager::Clear();

	ranStartup = false;
}

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	Shaders[name] = Shader(vShaderFile, fShaderFile, true);
	printf("ResourceManager.cpp::\t\tInitializing Shader\t'%s'\n", name.c_str());
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Shader* ResourceManager::GetShaderPtr(std::string name)
{
	return &Shaders[name];
}

Texture ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	Textures[name] = Texture(file, alpha);
	printf("ResourceManager.cpp::\t\tInitializing Texture\t'%s' (%s)\n",name.c_str(), file);
	return Textures[name];
}

Texture ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

Texture* ResourceManager::GetTexturePtr(std::string name)
{
	return &Textures[name];
}


Item ResourceManager::LoadItem(ItemType key, std::string itemName, std::string description, bool autoUse, bool canStack)
{
	Items[key] = Item(key, itemName.c_str(), description.c_str(), autoUse, canStack);
	//Items.emplace(key, new Item(key, itemName.c_str(), description.c_str(), autoUse, canStack));
	return Items[key];
}

Item ResourceManager::GetItem(ItemType key)
{
	return Items[key];
}

Item* ResourceManager::GetItemPtr(ItemType key)
{
	return &Items[key];
}


void ResourceManager::Clear()
{
	// (properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.GetId());
	// (properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.id);
	//for (auto iter : Items)
	//{
	//	delete iter.second;
	//}

	Shaders.clear();
	Textures.clear();
	Items.clear();
	lookupItemToTexture.clear();

}