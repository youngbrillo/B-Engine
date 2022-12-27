#pragma once
#ifndef BASECONTACTLISTENER_H
#define BASECONTACTLISTENER_H

#include <Box2D/Box2D.h>
#include "Game.h"
class baseContactListener : public b2ContactListener
{
public:

	baseContactListener(Game* g) : b2ContactListener(), currentGame(g) {}
	Game* currentGame;
	// Callbacks for derived classes.
	virtual void BeginContact(b2Contact* contact)  override;
	virtual void EndContact(b2Contact* contact)  override;
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

};

#endif // !BASECONTACTLISTENER_H
