#include "defaults_SpaceBuster.h"
#include "ResourceManager.h"


GameObjectDefinition DefaultItemDefinition(const b2Vec2& nextItemPos, const b2Vec2& nextItemSpeed, SpaceBuster::ItemType currentItemType)
{

	GameObjectDefinition DEFAULT_ITEM_DEF;
	DEFAULT_ITEM_DEF.position = glm::vec3(nextItemPos.x, nextItemPos.y, 0.0f);
	DEFAULT_ITEM_DEF.enabled_physics = true;
	DEFAULT_ITEM_DEF.scale = 3.0f;


	b2BodyDef bd;
	bd.type = b2BodyType::b2_dynamicBody;
	bd.position = b2Vec2(nextItemPos.x, nextItemPos.y);
	bd.linearVelocity = nextItemSpeed;
	bd.fixedRotation = true;

	DEFAULT_ITEM_DEF.objBodydef = &bd;

	b2FixtureDef fd;
	fd.isSensor = true;
	//b2CircleShape shape;
	//shape.m_radius = 1.0f;
	b2PolygonShape shape;
	shape.SetAsBox(DEFAULT_ITEM_DEF.scale * DEFAULT_ITEM_DEF.size.x / 2, DEFAULT_ITEM_DEF.scale * DEFAULT_ITEM_DEF.size.y / 2);
	fd.shape = &shape;

	DEFAULT_ITEM_DEF.objFixDef = &fd;
	DEFAULT_ITEM_DEF.texture = ResourceManager::GetTexturePtr(ResourceManager::lookupItemToTexture[currentItemType]);


	return DEFAULT_ITEM_DEF;
}
