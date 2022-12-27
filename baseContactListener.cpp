#include "baseContactListener.h"

void baseContactListener::BeginContact(b2Contact* contact)
{
	if (currentGame)
		currentGame->onBeginContact(contact);
}

void baseContactListener::EndContact(b2Contact* contact)
{
	if (currentGame)
		currentGame->onEndContact(contact);
}

void baseContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	if (currentGame)
		currentGame->onPreSolve(contact, oldManifold);
}

void baseContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	if (currentGame)
		currentGame->onPostSolve(contact, impulse);
}
