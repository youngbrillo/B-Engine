#include "ObjectFactory.h"


std::vector<GameObject*> ObjectFactory::ALLGAMEOBJECTS;
ObjectListener* ObjectFactory::CURRENT_OBJECT_LISTENER = nullptr;

void ObjectFactory::InsertObject(GameObject* newObject)
{
	ALLGAMEOBJECTS.emplace_back(newObject);
	if (CURRENT_OBJECT_LISTENER != nullptr)
	{
		CURRENT_OBJECT_LISTENER->ObjectCreated(newObject);
	}
}

std::vector<GameObject*>::iterator ObjectFactory::DestroyObject(GameObject* existingObject)
{
	std::vector<GameObject*>::iterator next;
	next = ALLGAMEOBJECTS.erase(std::remove(ALLGAMEOBJECTS.begin(), ALLGAMEOBJECTS.end(), existingObject), ALLGAMEOBJECTS.end());

	if (CURRENT_OBJECT_LISTENER != nullptr)
	{
		CURRENT_OBJECT_LISTENER->ObjectDeleted(existingObject);
	}
	delete existingObject;
	existingObject = nullptr;

	return next;
}

void ObjectFactory::UpdateObjects(float dt)
{
	auto obj = ALLGAMEOBJECTS.begin();
	while (obj != ALLGAMEOBJECTS.end())
	{
		(*obj)->Update(dt);
		if ((*obj)->deletionFlag)
		{
			obj = DestroyObject((*obj));
		}
		else
		{
			obj++;
		}
	}
}

void ObjectFactory::FixedUpdateObjects(float dt)
{
	auto obj = ALLGAMEOBJECTS.begin();
	while (obj != ALLGAMEOBJECTS.end())
	{
		(*obj)->FixedUpdate(dt);
		obj++;
	}
}

void ObjectFactory::handleBeginContact(b2Contact* contact)
{
	auto obj = ALLGAMEOBJECTS.begin();
	while (obj != ALLGAMEOBJECTS.end())
	{
		(*obj)->handleBeginContact(contact);
		obj++;
	}
}

void ObjectFactory::handleEndContact(b2Contact* contact)
{
	auto obj = ALLGAMEOBJECTS.begin();
	while (obj != ALLGAMEOBJECTS.end())
	{
		(*obj)->handleEndContact(contact);
		obj++;
	}
}

void ObjectFactory::handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	auto obj = ALLGAMEOBJECTS.begin();
	while (obj != ALLGAMEOBJECTS.end())
	{
		(*obj)->handlePreSolve(contact, oldManifold);
		obj++;
	}
}

void ObjectFactory::handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	auto obj = ALLGAMEOBJECTS.begin();
	while (obj != ALLGAMEOBJECTS.end())
	{
		(*obj)->handlePostSolve(contact, impulse);
		obj++;
	}
}

void ObjectFactory::DrawObjects(Shader* globalSpriteShader, Surface* globalSpriteSurface)
{
	auto obj = ALLGAMEOBJECTS.begin();
	while (obj != ALLGAMEOBJECTS.end())
	{
		(*obj)->Draw(globalSpriteShader, globalSpriteSurface);
		obj++;
	}
}

void ObjectFactory::cleanup()
{
	auto obj = ALLGAMEOBJECTS.begin();
	while (obj != ALLGAMEOBJECTS.end())
	{
		obj = DestroyObject((*obj));
	}
	if (ALLGAMEOBJECTS.size() != 0)
	{
		printf("OBJECTFACTORY:: Did not properly Destroy All Game Objects\n");
	}

	ALLGAMEOBJECTS.clear();
}

void ObjectFactory::SetObjectListener(ObjectListener* newObjectListener)
{
	CURRENT_OBJECT_LISTENER = newObjectListener;
}

#include "imgui/imgui.h"

void ObjectFactory::Debug()
{
	if (ImGui::TreeNode("- OBJECT FACTORY -"))
	{
		for (auto i : ObjectFactory::ALLGAMEOBJECTS)
		{
			i->Debug();
		}

		ImGui::TreePop();
	}
}
