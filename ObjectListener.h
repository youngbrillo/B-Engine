#pragma once
#ifndef OBJECTLISTENER_H
#define OBJECTLISTENER_H

#include "GameObject.h"

class ObjectListener
{
public:
	ObjectListener() {};
	virtual ~ObjectListener() {};
	virtual void ObjectCreated(GameObject* G) = 0;
	virtual void ObjectDeleted(GameObject* G) = 0;
protected:
};

#endif
