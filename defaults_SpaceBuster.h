#pragma once
#ifndef DEFAULTS_SPACEBUSTER_H
#define DEFAULTS_SPACEBUSTER_H

#include "GameObject.h"
#include "Item.h"

GameObjectDefinition DefaultItemDefinition(const b2Vec2& nextItemPos, const b2Vec2& nextItemSpeed, SpaceBuster::ItemType currentItemType);

#endif // !DEFAULTS_SPACEBUSTER_H
